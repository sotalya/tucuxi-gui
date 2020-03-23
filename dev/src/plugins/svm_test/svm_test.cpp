#include "svm_test.h"

#include "ezxmlreader.h"
#include "core_errors.h"
#include "coremessagehandler.h"
#include "convert.h"

#include <QtPlugin>

#include <math.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include <gsl/gsl_blas.h>
#include <gsl/gsl_multifit.h>

struct patientStruct {
    int num;
    float * concentrations;
    float * times;
    float * doses;
    int size; // Number of entries in the concentrations, times, and doses arrays
    int sex;
    float age;
    float weight;
};

typedef struct patientStruct Patient;

struct databaseStruct {
    Patient * patients;
    int size; // Number of patients
};

typedef struct databaseStruct SvmDatabase;

struct svmStruct {
    double means[5]; // Normalization constants
    double stds[5]; // Normalization constants
    double sigma; // Gaussian kernel
    double C; // Regularization
    gsl_matrix * trainFeat; // Training samples (support vectors) already normalized
    gsl_vector * alpha; // Trained coefficients
};

typedef struct svmStruct SVM;

// Allocate the concentrations, times, and doses arrays
void createPatient(Patient * p, int size);

// Free the concentrations, times, and doses arrays
void deletePatient(Patient * p);

// Read a database from a file
SvmDatabase readDatabase(const char * filename);

// Print a database
void printDatabase(const SvmDatabase * db);

// Free a database
void deleteDatabase(SvmDatabase * db);

// Returns the number of inliners, the 4 alpha coefficients, and the indices of the inliners.
// The inliners array contains the indices of the inliners and must be already allocated with the same size as x and y.
int ransac(const float * x, const float * y, int size, float threshold, int k, float alpha[4], int * inliners);

void predictGaussianSVM(const gsl_matrix * xTrain, const gsl_matrix * xTest, const gsl_vector * alpha, double sigma, gsl_vector * y);

void trainGaussianSVM(const gsl_matrix * xTrain, const gsl_vector * y, double * C, double * sigma, gsl_vector * alpha);

// Normalization
Patient * normalization(const Patient * patients);

// Return predicted concentrations for certain time
int predictN(double start, double stop, int n, const Patient * p, float dose, const SVM * svm, gsl_vector * out);

int mainSvm (const char *fileTrain,
             const char *fileTest,
             double startTime,
             double untilTime,
             int nbPoints,
             double dosage,
             gsl_vector * out) {
    // set up the structure of each patient
//	Patient patLib[200];

    // read in the data
//	Patient p[200];

    // RANSAC
//	float threshold = 400;
//	int k = 10;
//	Patient * inlinerP;
//	inlinerP = ransac(p, threshold, k);

    // Read in the data
    printf("Training database:\n");
    SvmDatabase dbtrain = readDatabase(fileTrain);
    printDatabase(&dbtrain);

    printf("\nTesting database:\n");
    SvmDatabase dbtest = readDatabase(fileTest);
    printDatabase(&dbtest);

    // normalization
//	for i = 1:size(trainFeat,2)
//		m1 = mean(trainFeat(:,i));
//    s1 = std(trainFeat(:,i));
//    if s1 ~= 0
//        trainFeat(:,i) = (trainFeat(:,i)-m1)/s1;
//	testFeat(:,i) = (testFeat(:,i)-m1)/s1;

    // Create the trainFeat matrix + RANSAC
    int nbSamplesTrain = 0;

    for (int i = 0; i < dbtrain.size; ++i)
        nbSamplesTrain += dbtrain.patients[i].size;

    int nbSamplesTest = 0;

    for (int i = 0; i < dbtest.size; ++i)
        nbSamplesTest += dbtest.patients[i].size;

    gsl_matrix * trainFeat = gsl_matrix_alloc(nbSamplesTrain, 5);
    gsl_matrix * testFeat = gsl_matrix_alloc(nbSamplesTest, 5);

    float * x = (float *)malloc(nbSamplesTrain * sizeof(float));
    float * y = (float *)malloc(nbSamplesTrain * sizeof(float));

    for (int i = 0, j = 0; i < dbtrain.size; ++i) {
        for (int k = 0; k < dbtrain.patients[i].size; ++k, ++j) {
            gsl_matrix_set(trainFeat, j, 0, dbtrain.patients[i].times[k]);
            gsl_matrix_set(trainFeat, j, 1, dbtrain.patients[i].doses[k]);
            gsl_matrix_set(trainFeat, j, 2, dbtrain.patients[i].sex);
            gsl_matrix_set(trainFeat, j, 3, dbtrain.patients[i].age);
            gsl_matrix_set(trainFeat, j, 4, dbtrain.patients[i].weight);

            x[j] = dbtrain.patients[i].times[k];
            y[j] = dbtrain.patients[i].concentrations[k];
        }
    }

    for (int i = 0, j = 0; i < dbtest.size; ++i) {
        for (int k = 0; k < dbtest.patients[i].size; ++k, ++j) {
            gsl_matrix_set(testFeat, j, 0, dbtest.patients[i].times[k]);
            //gsl_matrix_set(testFeat, j, 0, 24);
            gsl_matrix_set(testFeat, j, 1, dbtest.patients[i].doses[k]);
            gsl_matrix_set(testFeat, j, 2, dbtest.patients[i].sex);
            gsl_matrix_set(testFeat, j, 3, dbtest.patients[i].age);
            gsl_matrix_set(testFeat, j, 4, dbtest.patients[i].weight);
        }
    }

    float alpha[3];
    int * inliners = (int *)malloc(nbSamplesTrain * sizeof(int));

    int nbInliners = ransac(x, y, nbSamplesTrain, 200.0f, 3, alpha, inliners);

    printf("# inliners = %d / %d, alpha = %f %f %f\n", nbInliners, nbSamplesTrain, alpha[0], alpha[1], alpha[2]);


    for (int i = 0; i < nbInliners; ++i)
        for (int j = 0; j < 5; ++j)
            gsl_matrix_set(trainFeat, i, j, gsl_matrix_get(trainFeat, inliners[i], j));

    trainFeat->size1 = nbInliners;
    nbSamplesTrain = trainFeat->size1;

    SVM svm;

    // Normalize the trainFeat matrix
    for (int i = 0; i < 5; ++i) {
        float mean=0;
        float std=0;

        for (int j = 0; j < nbSamplesTrain; ++j) {
            float x = gsl_matrix_get(trainFeat, j, i);
            mean += x;
            std += x * x;
        }

        mean /= nbSamplesTrain;
        std = sqrt(std / nbSamplesTrain - mean * mean);

        printf("Feature %d, mean %f, std %f\n", i, mean, std);

        for (int j = 0; j < nbSamplesTrain; ++j)
            gsl_matrix_set(trainFeat, j, i, (gsl_matrix_get(trainFeat, j, i) - mean) / std);

        for (int j = 0; j < nbSamplesTest; ++j)
            gsl_matrix_set(testFeat, j, i, (gsl_matrix_get(testFeat, j, i) - mean) / std);

        svm.means[i] = mean;
        svm.stds[i] = std;
    }

    // SVM
    gsl_vector * y2 = gsl_vector_calloc(nbSamplesTrain);
    gsl_vector * a = gsl_vector_calloc(nbSamplesTrain);
//	gsl_vector * out = gsl_vector_calloc(nbSamplesTest);
    //gsl_vector * signal = gsl_vector_calloc(nbSamplesTest);

    for (int j = 0; j < nbSamplesTrain; ++j)
        gsl_vector_set(y2, j, y[j]);

    svm.C = 1;
    svm.sigma = 1;

    trainGaussianSVM(trainFeat, y2, &svm.C, &svm.sigma, a);

//	printf("alpha =");
//	for (int j = 0; j < nbSamplesTrain; ++j)
//		printf(" %f", gsl_vector_get(a,j));
//	printf(", C = %f, sigma = %f\n", C, sigma);

    // Predict the drug concentration for all the patients in complete_test.txt file
//	predictGaussianSVM(trainFeat, testFeat, a, svm.sigma, out);

    ////////////////////////////// predictN() //////////////////////////////////////////////////////
    // Predict the drug concentrations w.r.t a certain start time, stop time, and sampling period.
    // Currently, 'p' is defined by given a patient number (0~73) referring to the complete_test.txt.
    // 'out' gives the predicted concentration values.
    double start = startTime;
    double stop = untilTime;
    //int period = 24;
    int period = nbPoints;
    float dose = dosage;
    Patient p = dbtest.patients[65];
    svm.trainFeat = trainFeat;
    svm.alpha = a;

    int result =0;
    if (predictN(start, stop, period, &p, dose, &svm, out))
        result = -1;


    // recommendations
    gsl_vector_free(y2);
    gsl_vector_free(a);
    gsl_matrix_free(trainFeat);
    gsl_matrix_free(testFeat);
    free(x);
    free(y);
    free(inliners);

    return result;
}

void createPatient(Patient * p, int size)
{
    p->concentrations = (float *)malloc(size * sizeof(float));
    p->times = (float *)malloc(size * sizeof(float));
    p->doses = (float *)malloc(size * sizeof(float));
    p->size = size;
}

void deletePatient(Patient * p)
{
    free(p->concentrations);
    free(p->times);
    free(p->doses);
    p->concentrations = NULL;
    p->times = NULL;
    p->doses = NULL;
    p->size = 0;
}

SvmDatabase readDatabase(const char * filename)
{
    SvmDatabase db = {NULL, 0};
    Patient p = {-1, NULL, NULL, NULL, 0, 0, 0, 0};
    FILE * file = fopen(filename, "r");

    if (file == NULL) {
        fprintf(stderr, "Could not open file %s.\n", filename);
        return db;
    }

    while (!feof(file)) {
        // Try to read a whole line
        int num;
        float concentration;
        float time;
        float dose;
        float sex;
        float age;
        float weight;
        fscanf(file, "%d %f %f %f %f %f %f\n", &num, &concentration, &time, &dose, &sex, &age, &weight);

        if (!ferror(file)) {
            if (num == p.num) { // Still the same patient
                ++p.size;
                p.concentrations = (float *)realloc(p.concentrations, p.size * sizeof(float));
                p.times = (float *)realloc(p.times, p.size * sizeof(float));
                p.doses = (float *)realloc(p.doses, p.size * sizeof(float));
                p.concentrations[p.size - 1] = concentration;
                p.times[p.size - 1] = time;
                p.doses[p.size - 1] = dose;
            }
            else { // A new patient
                // Add the previous patient to the database if there is one
                if (p.num != -1) {
                    ++db.size;
                    db.patients = (Patient*)realloc(db.patients, db.size * sizeof(Patient));
                    db.patients[db.size - 1] = p;
                }

                // Create a new patient
                createPatient(&p, 1);
                p.num = num;
                p.concentrations[0] = concentration;
                p.times[0] = time;
                p.doses[0] = dose;
                p.sex = (int)sex;
                p.age = age;
                p.weight = weight;
            }
        }
    }

    // Add the previous patient to the database if there is one
    if (p.num != -1) {
        ++db.size;
        db.patients = (Patient *)realloc(db.patients, db.size * sizeof(Patient));
        db.patients[db.size - 1] = p;
    }


    // Create a new patient
    createPatient(&p, 0);
    p.num = 1000;
    p.sex = (int)1;
    p.age = 50;
    p.weight = 170;
    ++db.size;
    db.patients = (Patient *)realloc(db.patients, db.size * sizeof(Patient));
    db.patients[db.size - 1] = p;
    printf("Last patient : %d\n",db.size);

    fclose(file);

    return db;
}

void deleteDatabase(SvmDatabase * db)
{
    int i;

    for (i = 0; i < db->size; ++i)
        deletePatient(&db->patients[i]);

    free(db->patients);
    db->patients = NULL;
    db->size = 0;
}

void printDatabase(const SvmDatabase * db)
{
    int i, j;

    for (i = 0; i < db->size; ++i) {
        printf("Patient %d: sex %d, age %f, weight %f\n    concentrations:", db->patients[i].num,
               db->patients[i].sex, db->patients[i].age, db->patients[i].weight);

        for (j = 0; j < db->patients[i].size; ++j)
            printf(" %f", db->patients[i].concentrations[j]);

        printf("\n    times:");

        for (j = 0; j < db->patients[i].size; ++j)
            printf(" %f", db->patients[i].times[j]);

        printf("\n    doses:");

        for (j = 0; j < db->patients[i].size; ++j)
            printf(" %f", db->patients[i].doses[j]);

        printf("\n");
    }
}

// function [alpha, i] = ransac(x,y,th,k)
// m = [x x.^2 x.^3 (1-exp(-x))];
// inliners = 0;
// for i = 1:100000
//   r = randperm(size(x,1));
//   a = m(r(1:k),:) \ y(r(1:k));
//   n = sum(abs(m * a - y) < th);
//   if n > inliners
//     inliners = n;
//     alpha = a;
//   end
// end
// i = abs(m * alpha - y) < th;
// fprintf('#inliners = %d\n',sum(i));
// alpha = m(i,: ) \ y(i);
int ransac(const float * x, const float * y, int size, float threshold, int k, float * alpha, int * inliners)
{
    gsl_multifit_linear_workspace * work = gsl_multifit_linear_alloc(k, 3); // Required by GSL
    gsl_matrix * mx = gsl_matrix_alloc(size, 3); // Matlab: m
    gsl_matrix * mx2 = gsl_matrix_alloc(k, 3); // Matlab: m(r(1:k),:)
    gsl_vector * my = gsl_vector_alloc(k); // Matlab: y(r(1:k))
    gsl_vector * malpha = gsl_vector_alloc(3); // Matlab: a
    gsl_matrix * mcov = gsl_matrix_alloc(3, 3); // Required by GSL
    gsl_vector * mdist = gsl_vector_alloc(size); // Matlab: m * a
    int * randperm = (int *)malloc(k * sizeof(int)); // Matlab: r
    double chisq;
    int i, j, n, nbInliners;

    // Fill the matrix mx
    // Matlab: m = [x.^(-2) log(x) (1-exp(-x))];
    for (j = 0; j < size; ++j) {
        gsl_matrix_set(mx, j, 0, pow(x[j],-2));
        gsl_matrix_set(mx, j, 1, log(x[j]));
        gsl_matrix_set(mx, j, 2, 1.0 - exp(-x[j]));
    }

    // Matlab: inliners = 0;
    nbInliners = 0;

    // Matlab: for i = 1:100000
    for (i = 0; i < 100000; ++i) {
        // Sample k indices
        // Matlab: r = randperm(size(x,1));
        for (j = 0; j < k; ++j)
            randperm[j] = rand() % size;

        // Fill the matrix mx2 and the vector my
        // Matlab: m(r(1:k),:)
        // Matlab: y(r(1:k))
        for (j = 0; j < k; ++j) {
            gsl_matrix_set(mx2, j, 0, gsl_matrix_get(mx, randperm[j], 0));
            gsl_matrix_set(mx2, j, 1, gsl_matrix_get(mx, randperm[j], 1));
            gsl_matrix_set(mx2, j, 2, gsl_matrix_get(mx, randperm[j], 2));
            gsl_vector_set(my, j, y[randperm[j]]);
        }

        // Matlab: a = m(r(1:k),:) \ y(r(1:k));
        gsl_multifit_linear(mx2, my, malpha, mcov, &chisq, work);

        // Count the number of inliners
        // Matlab: m * a
        gsl_blas_dgemv(CblasNoTrans, 1.0, mx, malpha, 0.0, mdist);

        // Matlab: n = sum(abs(m * a - y) < th);
        n = 0;

        for (j = 0; j < size; ++j)
            if (fabs(gsl_vector_get(mdist, j) - y[j]) < threshold)
                ++n;

        // Matlab: if n > inliners
        if (n > nbInliners) {
            printf("RANSAC trial %d, # inliners = %d, chisq = %f, alpha = %f %f %f\n", i, n, chisq,
                   gsl_vector_get(malpha, 0), gsl_vector_get(malpha, 1), gsl_vector_get(malpha, 2));
            // Matlab: inliners = n;
            nbInliners = n;

            // Matlab: alpha = a;
            for (j = 0; j < 3; ++j)
                alpha[j] = gsl_vector_get(malpha, j);

            n = 0;

            for (j = 0; j < size; ++j)
                if (fabs(gsl_vector_get(mdist, j) - y[j]) < threshold)
                    inliners[n++] = j;
        }
    }

    gsl_multifit_linear_free(work);
    gsl_matrix_free(mx);
    gsl_matrix_free(mx2);
    gsl_vector_free(my);
    gsl_vector_free(malpha);
    gsl_matrix_free(mcov);
    gsl_vector_free(mdist);
    free(randperm);

    // TODO:
    // i = abs(m * alpha - y) < th;
    // fprintf('#inliners = %d\n',sum(i));
    // alpha = m(i,: ) \ y(i);

    return nbInliners;
}

// function [y] = predictGaussianSVM(Xtrain, Xtest, alpha, sigma)
// assert(size(Xtrain, 2) == size(Xtest, 2));
// N = size(Xtrain, 1);
// M = size(Xtest, 1);
// Xtrain2 = sum(Xtrain.^2, 2);
// Xtest2 = sum(Xtest.^2, 2);
// D = repmat(Xtest2, [1 N]) + repmat(Xtrain2', [M 1]) - 2 * (Xtest * Xtrain');
// if nargin < 4
//   sigma = sqrt(1 / mean(mean(D)));
// end
// K = exp(-D / (2 * sigma^2));
// y = K * alpha;
void predictGaussianSVM(const gsl_matrix * xTrain, const gsl_matrix * xTest, const gsl_vector * alpha, double sigma, gsl_vector * y)
{
    gsl_vector * xTrain2 = gsl_vector_calloc(xTrain->size1);
    gsl_matrix * kernel = gsl_matrix_alloc(xTest->size1, xTrain->size1);

    unsigned int i, j;

    assert(xTrain->size2 == xTest->size2); // Same number of features

    // Matlab: Xtrain2 = sum(Xtrain.^2, 2);
    for (i = 0; i < xTrain->size1; ++i) {
        double dot = 0.0;
        gsl_vector_const_view row = gsl_matrix_const_row(xTrain, i);
        gsl_blas_ddot(&row.vector, &row.vector, &dot);
        gsl_vector_set(xTrain2, i, dot);
    }

    // Matlab: Xtest2 = sum(Xtest.^2, 2);
    for (i = 0; i < xTest->size1; ++i) {
        double dot = 0.0;
        gsl_vector_const_view row = gsl_matrix_const_row(xTest, i);
        gsl_blas_ddot(&row.vector, &row.vector, &dot);

        // Matlab: D = repmat(Xtest2, [1 N]) + repmat(Xtrain2', [M 1])
        for (j = 0; j < xTrain->size1; ++j)
            gsl_matrix_set(kernel, i, j, gsl_vector_get(xTrain2, j) + dot);
    }

    // Matlab: - 2 * (Xtest * Xtrain');
    gsl_blas_dgemm(CblasNoTrans, CblasTrans,-2.0, xTest, xTrain, 1.0, kernel);

    if (sigma <= 0.0) {
        // Matlab: sigma = sqrt(1 / mean(mean(D)));
        double sum = 0.0;

        for (i = 0; i < xTest->size1; ++i)
            for (j = 0; j < xTrain->size1; ++j)
                sum += gsl_matrix_get(kernel, i, j);

        sigma = sum / (xTest->size1 * xTrain->size1);
    }

    // Matlab: K = exp(-D / (2 * sigma^2));
    sigma =-1.0 / (2.0 * sigma * sigma);

    for (i = 0; i < xTest->size1; ++i)
        for (j = 0; j < xTrain->size1; ++j)
            gsl_matrix_set(kernel, i, j, exp(gsl_matrix_get(kernel, i, j) * sigma));

    // Matlab: y = K * alpha;
    gsl_blas_dgemv(CblasNoTrans, 1.0, kernel, alpha, 0.0, y);

    gsl_vector_free(xTrain2);
    gsl_matrix_free(kernel);
}

// function [alpha] = trainGaussianSVM(X, y, C, sigma)
// N = size(X, 1);
// assert(size(y, 1) == N);
// assert(size(y, 2) == 1);
// X2 = sum(X.^2, 2);
// D = repmat(X2, [1 N]) + repmat(X2', [N 1]) - 2 * (X * X');
// if nargin < 3
//   C = 1000;
// end
// if nargin < 4
//   sigma = sqrt(1 / mean(mean(D)));
// end
// K = exp(-D / (2 * bestSigma^2));
// alpha = pinv(K + 1/bestC * eye(N)) * y;
void trainGaussianSVM(const gsl_matrix * xTrain, const gsl_vector * y, double * C, double * sigma, gsl_vector * alpha)
{
    gsl_matrix * kernel = gsl_matrix_calloc(xTrain->size1, xTrain->size1);
    gsl_multifit_linear_workspace * work = gsl_multifit_linear_alloc(xTrain->size1, xTrain->size1);
    gsl_matrix * cov = gsl_matrix_alloc(xTrain->size1, xTrain->size1);
    double chisq;
    unsigned int i, j;

    assert(y->size == xTrain->size1);

    // Matlab: X2 = sum(X.^2, 2);
    // Matlab: D = repmat(X2, [1 N]) + repmat(X2', [N 1])
    for (i = 0; i < xTrain->size1; ++i) {
        double dot = 0.0;
        gsl_vector_const_view row = gsl_matrix_const_row(xTrain, i);
        gsl_blas_ddot(&row.vector, &row.vector, &dot);

        for (j = 0; j < xTrain->size1; ++j) {
            gsl_matrix_set(kernel, i, j, gsl_matrix_get(kernel, i, j) + dot);
            gsl_matrix_set(kernel, j, i, gsl_matrix_get(kernel, j, i) + dot);
        }
    }

    // Matlab: - 2 * (X * X');
    gsl_blas_dgemm(CblasNoTrans, CblasTrans,-2.0, xTrain, xTrain, 1.0, kernel);

    if (*C <= 0.0)
        *C = 1000.0;

    if (*sigma <= 0.0) {
        double sum = 0.0;

        for (i = 0; i < xTrain->size1; ++i)
            for (j = 0; j < xTrain->size1; ++j)
                sum += gsl_matrix_get(kernel, i, j);

        *sigma = sum / (xTrain->size1 * xTrain->size1);
    }

    double sigmaInv =-1.0 / (2.0 * (*sigma) * (*sigma));
    double Cinv = 1.0 / *C;

    for (i = 0; i < xTrain->size1; ++i) {
        for (j = 0; j < xTrain->size1; ++j)
            gsl_matrix_set(kernel, i, j, exp(gsl_matrix_get(kernel, i, j) * sigmaInv));

        gsl_matrix_set(kernel, i, i, gsl_matrix_get(kernel, i, i) + Cinv);
    }

    gsl_multifit_linear(kernel, y, alpha, cov, &chisq, work);

    gsl_matrix_free(kernel);
    gsl_multifit_linear_free(work);
    gsl_matrix_free(cov);
}

int predictN(double start, double stop, int n, const Patient * p, float dose, const SVM * svm, gsl_vector * out)
{
    if ((start < 0) || (start >= stop) || (n < 1) || !p || !svm || !svm->trainFeat || !svm->alpha ||
        (svm->trainFeat->size1 != svm->alpha->size) || (svm->sigma <= 0) || !out)
        return -1;

    gsl_matrix * testFeat = gsl_matrix_alloc(n, 5);

    // Normalize the testFeat matrix
    for (int j = 0; j < n; ++j)
        gsl_matrix_set(testFeat, j, 0, (start + j * (stop - start) / n - svm->means[0]) / svm->stds[0]);

    for (int j = 0; j < n; ++j)
        gsl_matrix_set(testFeat, j, 1, (dose - svm->means[1]) / svm->stds[1]);

    for (int j = 0; j < n; ++j)
        gsl_matrix_set(testFeat, j, 2, (p->sex - svm->means[2]) / svm->stds[2]);

    for (int j = 0; j < n; ++j)
        gsl_matrix_set(testFeat, j, 3, (p->age - svm->means[3]) / svm->stds[3]);

    for (int j = 0; j < n; ++j)
        gsl_matrix_set(testFeat, j, 4, (p->weight - svm->means[4]) / svm->stds[4]);


    predictGaussianSVM(svm->trainFeat, testFeat, svm->alpha, svm->sigma, out);

    printf("\nout:");
    for (int i = 0; i < n; ++i)
        printf(" %f", gsl_vector_get(out, i));

    gsl_matrix_free(testFeat);

    printf("\n\n fen ge xian\n\n");
    return 0;
}



#include <gsl/gsl_blas.h>

SvmPredictFactory::SvmPredictFactory() : _pluginId(SVMPRED_ID)
{
}

// Set the possible options and parameters
void SvmPredictFactory::init (Core *core)
{
    //Set the core
    CORE->setInstance(core);

    //Create the models
    QList<Model*> models;
    models.append(new SvmPredict());

    //Store the descriptions
    for (int i = 0; i < models.size(); i++)
        _models.append(models.at(i)->descriptor());

    //Store the priorities
    for (int i = 0; i < models.size(); i++)
        _priorities.insert(models.at(i)->descriptor().id, models.at(i)->priority());

    //Delete the models
    qDeleteAll(models);
}

//Returns the plugin ID
QString SvmPredictFactory::pluginId()
{
    return _pluginId;
}

//Create the corresponding new model
Model *SvmPredictFactory::newModel(QString id)
{
    if (id == _models.at(0).id)
        return new SvmPredict();
    else
        return 0;
}












#define TRAININGFILE_OPTION "1trainingfile"
#define TESTFILE_OPTION     "2testfile"

//The model default priority level
const int SvmPredict::_priority = 100;

SvmPredict::SvmPredict()
{
    //We want ug for the dose
    _ug.fromString("ug");
    //And ug/l for the result
    _ugl.fromString("ug/l");

    // Parameters

    _parameters.append(Descriptor("a1",tr("a1...*(pow(T,-2))"),tr("First parameter")));
    _parameters.append(Descriptor("a2",tr("a2...*log(T)"),tr("Second parameter")));
    _parameters.append(Descriptor("a3",tr("a3...*(1-exp(-T))"),tr("Third parameter")));


    //Name of the file containing the training data
    addOption(TRAININGFILE_OPTION, tr("Training data"), tr("File containing the training data"),
              qMetaTypeId<FileName>(), QVariant::fromValue(FileName("","*.txt")));

    //Name of the file containing the test data
    addOption(TESTFILE_OPTION, tr("Test data"), tr("File containing the test data"),
              qMetaTypeId<FileName>(), QVariant::fromValue(FileName("","*.txt")));
}

//Copy ourselves, and our options
Model *SvmPredict::clone()
{
    Model *other = new SvmPredict();
    other->copyOptions(this);
    return other;
}

//Create it here
ModelDescriptor SvmPredict::descriptor()
{
    return ModelDescriptor ( SVMPRED1_ID,
                             tr("SVM Test"),
                             tr("Support Vector Machine, from EPFL"),
                             "extra" );
}

//This list is created in the constructor
QList<Descriptor> SvmPredict::parametersDescription()
{
    return _parameters;
}

int SvmPredict::priority() const
{
    return _priority;
}

//Yeah!
Version SvmPredict::version()
{
    return Version("0.1");
}

//Return our error string
QString SvmPredict::errorString()
{
    return _errors;
}

//Static (that's what the CHUV said)
Duration SvmPredict::halflife(const Duration &, const ParameterSet &/*parameters*/, const Dosage &, const Residuals &)
{
    //Log(2) static value
#define LOG2H 0.693147180559945*3600000.0
    // t½ = ln(2) / (CL/V)
    return Duration(0,0,0,LOG2H / 100);
#undef LOG2H
}


//Simple formula application
DataSet SvmPredict::predict(const Duration &start, const Duration &until, const ParameterSet &parameters, uint points, const Dosage &/*dosage*/, Residuals &residuals)
{

    FileName trainingFile = option(TRAININGFILE_OPTION).value.value<FileName>();
    FileName testFile = option(TESTFILE_OPTION).value.value<FileName>();

    if (trainingFile.isEmpty())
        trainingFile = FileName(QCoreApplication::applicationDirPath() + "/" + DB_TRAIN_PATH);
    if (testFile.isEmpty())
        testFile = FileName(QCoreApplication::applicationDirPath() + "/" + DB_TEST_PATH);

    //Fetch the parameters
    double a1 = parameters.get("a1").value;
    double a2 = parameters.get("a2").value;
    double a3 = parameters.get("a3").value;

    //Starting time
    double T = start.toHours();

    //Delta
    double delta = until.toHours() / points;

    //Result
    DataSet data(points,_ugl);

    //Calculate each point
    for (uint i=0; i < points; i++) {
        T += delta;
        double T2 = T;
        data[i] = a1*(pow(T2,-2))+a2*log(T2)+a3*(1-exp(-T2));
    }

    //Set the new residual
    residuals[0] = Residual(data[points-1]);

    return data;
}


//List created in init()
QList<ModelDescriptor> SvmPredictFactory::models()
{
    return _models;
}

//Returns a model priority
int SvmPredictFactory::priority(const QString &id) const
{
    return _priorities.value(id, -1);
}

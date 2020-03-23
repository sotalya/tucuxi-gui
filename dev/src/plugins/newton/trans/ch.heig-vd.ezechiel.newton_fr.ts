<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="fr_FR">
<context>
    <name>NewtonReverse</name>
    <message>
        <location filename="../newtonreverse.cpp" line="52"/>
        <source>Delta</source>
        <translation>Delta</translation>
    </message>
    <message>
        <location filename="../newtonreverse.cpp" line="52"/>
        <source>Difference between two point for the numerical derivation, should be small</source>
        <translation>Différence entre deux points lors de la dérivation numérique. Doit être petit</translation>
    </message>
    <message>
        <location filename="../newtonreverse.cpp" line="56"/>
        <source>Maximal difference (%)</source>
        <translation>Différence maximale (%)</translation>
    </message>
    <message>
        <location filename="../newtonreverse.cpp" line="56"/>
        <source>Maximal difference between the wanted concentration and the concentration found with the algorithm, in percent</source>
        <translation>Différence maximale entre la concentration trouvée et la concentration voulue, en pourcent. Lorsque cette valeur est atteinte, l&apos;algorithme se termine</translation>
    </message>
    <message>
        <location filename="../newtonreverse.cpp" line="60"/>
        <source>Maximum number of iteration</source>
        <translation>Nombre maximum d&apos;itérations</translation>
    </message>
    <message>
        <location filename="../newtonreverse.cpp" line="60"/>
        <source>In each iteration, the algorithm will make some prediction and see if it matches the wanted concentration, to then adapt the dosage. This value permits to fix a maximal number of iteration to do, avoiding an infinite loop</source>
        <translation>A chaque itération l&apos;algorithme doit effectuer une prédiction, vérifier si elle permet d&apos;atteindre la concentration désirée, et adapter le dosage si oui. Cette valeur permet de fixer un nombre maximum d&apos;itération, afin d&apos;éviter une boucle de calcul infinie</translation>
    </message>
    <message>
        <location filename="../newtonreverse.cpp" line="79"/>
        <source>Newton</source>
        <translation>Newton</translation>
    </message>
    <message>
        <location filename="../newtonreverse.cpp" line="80"/>
        <source>Reverse calculation using the Newton&apos;s method to find the root of the function.
This engine uses only the residual target, if not given, the result will be the same as the given value.</source>
        <translation>Proposition de posologie exploitant la méthode de Newton pour trouver le dosage optimal.
Cet engin n&apos;utilise que la cible résiduelle.</translation>
    </message>
    <message>
        <location filename="../newtonreverse.cpp" line="119"/>
        <source>No residual target found, cannot compute.</source>
        <translation>Aucune cible résiduelle trouvée, impossible de calculer une posologie.</translation>
    </message>
    <message>
        <location filename="../newtonreverse.cpp" line="144"/>
        <source>Maximum number of iteration reached when searching for the dosage, the result might be wrong</source>
        <translation>Le nombre maximum d&apos;itérations a été atteint lors de la recherche de posologie. Le résultat peut donc être erroné</translation>
    </message>
</context>
<context>
    <name>NewtonReverseFactory</name>
    <message>
        <location filename="../newtonreverse.cpp" line="29"/>
        <source>Newton</source>
        <translation>Newton</translation>
    </message>
    <message>
        <location filename="../newtonreverse.cpp" line="30"/>
        <source>Reverse calculation using the Newton&apos;s method to find the root of the function.
This engine uses only the residual target, if not given, the result will be the same as the given value.</source>
        <translation>Proposition de posologie exploitant la méthode de Newton pour trouver le dosage optimal.
Cet engin n&apos;utilise que la cible résiduelle.</translation>
    </message>
</context>
</TS>

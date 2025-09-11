

import {drawGraph, drawAnnotations} from './graphing.js';
import {test1, test2, testempty, testtarget0, testtarget1, testtarget2, testtarget3, testtarget5,
        testtarget6, testtarget7, testtarget8, testtarget9, testtarget10, testtarget11, testtarget12}
        from './test_graphing.js';


import { createCanvas, loadImage }  from 'canvas';
import fs  from 'fs';


async function runTest(testFunction, testnb, scale, width, height)
{
    var canvas = createCanvas(width,height);
        
    let {obj, expected} = testFunction();
    obj.scale = scale;

    obj.updateChartDimensions(canvas);
    obj.canvas = canvas;
    obj.annotationsCanvas = canvas;
    obj.clockCanvas = canvas;
  
    var mini_path = "covariates_disabled_mini.png";
    var img_covariates_disabled_mini = await loadImage(mini_path);
    var dosage_mini_path = "dosages_disabled_mini.png";
    var img_dosages_disabled_mini = await loadImage(dosage_mini_path);

    obj.img_covariates_disabled_mini = img_covariates_disabled_mini;
    obj.img_dosages_disabled_mini = img_dosages_disabled_mini;
        
    drawGraph(obj);
    drawAnnotations(obj);

    const buffer = canvas.toBuffer('image/png');
    fs.writeFileSync('output' + testnb + '.png', buffer);
};

function main()
{
    var allTests = [test1,
                test1,
                test1,
                test1,
                test1,
                test1,
                test1,
                test1,
                test2,
                testempty,
                testtarget0,
                testtarget1,
                testtarget2,
                testtarget3,
                testtarget5,
                testtarget6,
                testtarget7,
                testtarget8,
                testtarget9,
                testtarget10,
                testtarget11,
                testtarget12];
    runTest(allTests[0], 0, 1, 600, 400);
    runTest(allTests[1], 1, 2, 1200, 800);
    runTest(allTests[2], 2, 4, 2400, 1600);
    
    for(var tid = 3; tid < allTests.length; tid++) {
        runTest(allTests[tid], tid, 2, 1200, 800);
    }
}

main()

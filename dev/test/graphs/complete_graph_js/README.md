Test Graphing.js
================

The script gen_images.js allows to generate images based on the tests described in test_graphing.js.

To run the script you need node.js.

In this folder, run:

    npm install

Then start the test with:

    node gen_images.js


All required files are within the current folder. We can notice that graphing.js is also here, because it is
slightly different than the one used in Tucuxi-gui. It has to export two methods, drawGraph() and drawAnnotations(), 
and as such these methods have the export keyword on the front. It makes it incompatible with QML.

It means, if graphing.js would change, the one in this folder should be updated accordingly.

It could be automated if required, with a script taking the original graphind.js and adding the two keywords.

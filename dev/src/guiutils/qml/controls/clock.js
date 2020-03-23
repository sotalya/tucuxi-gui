//http://www.encodedna.com/html5/canvas/simple-analog-clock-using-canvas-javascript.htm
.import "graphing.js" as Graphing;

function showClock() {

    if (!popP) {return;}
    // DEFINE CANVAS AND ITS CONTEXT.
    var ctx = canvas.getContext('2d');

    var angle;
    var secHandLength = 10;

    var nowsecs = date.getTime() / 1000;
    var centerx = Graphing.atime2screen(nowsecs);
    //    var centerx = canvas.width * 0.5;
    var centery = canvas.height * 0.2;
    // CLEAR EVERYTHING ON THE CANVAS. RE-DRAW NEW ELEMENTS EVERY SECOND.
    //    ctx.clearRect(0, 0, canvas.width, canvas.height);

    function outer_dial1() {
        ctx.beginPath();
        ctx.arc(centerx, centery, secHandLength + 5, 0, Math.PI * 2);
        ctx.strokeStyle = '#92949C';
        ctx.stroke();
    }
    function outer_dial2() {
        ctx.beginPath();
        ctx.arc(centerx, centery, secHandLength + 4, 0, Math.PI * 2);
        ctx.strokeStyle = '#929BAC';
        ctx.stroke();
    }
    function center_dial() {
        ctx.beginPath();
        ctx.arc(centerx, centery, 2, 0, Math.PI * 2);
        ctx.lineWidth = 3;
        ctx.fillStyle = '#353535';
        ctx.strokeStyle = '#0C3D4A';
        ctx.stroke();
    }

    function mark_the_hours() {

        for (var i = 0; i < 12; i++) {
            angle = (i - 3) * (Math.PI * 2) / 12;       // THE ANGLE TO MARK.
            ctx.lineWidth = 1;            // HAND WIDTH.
            ctx.beginPath();

            var x1 = (centerx) + Math.cos(angle) * (secHandLength);
            var y1 = (centery) + Math.sin(angle) * (secHandLength);
            var x2 = (centerx) + Math.cos(angle) * (secHandLength - (secHandLength / 7));
            var y2 = (centery) + Math.sin(angle) * (secHandLength - (secHandLength / 7));

            ctx.moveTo(x1, y1);
            ctx.lineTo(x2, y2);

            ctx.strokeStyle = '#466B76';
            ctx.stroke();
        }
    }

    function mark_the_seconds() {

        for (var i = 0; i < 60; i++) {
            angle = (i - 3) * (Math.PI * 2) / 60;       // THE ANGLE TO MARK.
            ctx.lineWidth = 1;            // HAND WIDTH.
            ctx.beginPath();

            var x1 = (centerx) + Math.cos(angle) * (secHandLength);
            var y1 = (centery) + Math.sin(angle) * (secHandLength);
            var x2 = (centerx) + Math.cos(angle) * (secHandLength - (secHandLength / 30));
            var y2 = (centery) + Math.sin(angle) * (secHandLength - (secHandLength / 30));

            ctx.moveTo(x1, y1);
            ctx.lineTo(x2, y2);

            ctx.strokeStyle = '#C4D1D5';
            ctx.stroke();
        }
    }

    function show_seconds() {

        var sec = date.getSeconds();
        angle = ((Math.PI * 2) * (sec / 60)) - ((Math.PI * 2) / 4);
        ctx.lineWidth = 0.5;              // HAND WIDTH.

        ctx.beginPath();
        // START FROM CENTER OF THE CLOCK.
        ctx.moveTo(centerx, centery);
        // DRAW THE LENGTH.
        ctx.lineTo((centerx + Math.cos(angle) * secHandLength),
                   centery + Math.sin(angle) * secHandLength);

        // DRAW THE TAIL OF THE SECONDS HAND.
        ctx.moveTo(centerx, centery);    // START FROM CENTER.
        // DRAW THE LENGTH.
        ctx.lineTo((centerx - Math.cos(angle) * 20),
                   centery - Math.sin(angle) * 20);

        ctx.strokeStyle = '#586A73';        // COLOR OF THE HAND.
        ctx.stroke();
    }

    function show_minutes() {

        var min = date.getMinutes();
        angle = ((Math.PI * 2) * (min / 60)) - ((Math.PI * 2) / 4);
        ctx.lineWidth = 1.5;              // HAND WIDTH.

        ctx.beginPath();
        ctx.moveTo(centerx, centery);  // START FROM CENTER.
        // DRAW THE LENGTH.
        ctx.lineTo((centerx + Math.cos(angle) * secHandLength / 1.1),
                   centery + Math.sin(angle) * secHandLength / 1.1);

        ctx.strokeStyle = '#999';  // COLOR OF THE HAND.
        ctx.stroke();
    }

    function show_hours() {

        var hour = date.getHours();
        var min = date.getMinutes();
        angle = ((Math.PI * 2) * ((hour * 5 + (min / 60) * 5) / 60)) - ((Math.PI * 2) / 4);
        ctx.lineWidth = 1.5;              // HAND WIDTH.

        ctx.beginPath();
        ctx.moveTo(centerx, centery);     // START FROM CENTER.
        // DRAW THE LENGTH.
        ctx.lineTo((centerx + Math.cos(angle) * secHandLength / 1.5),
                   centery + Math.sin(angle) * secHandLength / 1.5);

        ctx.strokeStyle = '#000';   // COLOR OF THE HAND.
        ctx.stroke();
    }

    function draw_time_line() {
        ctx.lineWidth = 10;
        ctx.strokeStyle = '#000';
        ctx.globalAlpha = 0.1;
        ctx.beginPath();
        ctx.moveTo(centerx, centery + secHandLength + 5);
        ctx.lineTo(centerx, bottomLeftY);
        ctx.stroke();
    }

    outer_dial1();
    outer_dial2();
    center_dial();
    mark_the_hours();
    mark_the_seconds();

    show_seconds();
    show_minutes();
    show_hours();

    draw_time_line();

    overlay.requestPaint();
}

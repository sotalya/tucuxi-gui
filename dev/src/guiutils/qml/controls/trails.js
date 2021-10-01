
/**
 * adapted from http://hakim.se/experiments/
 * FC: wrapped all globally injected properties inside a "tdata" parameter passed to all functions.
 */

// eslint-disable-next-line @typescript-eslint/no-unused-vars
function init(tdata) {

    // FC: put overlay inside tdata instead of attaching extra properties to overlay
    if (tdata.overlay && tdata.overlay.getContext) {
        //var context = tdata.overlay.getContext('2d');
        createParticles(tdata);
    }
}

function createParticles(tdata) {
    tdata.particles = [];

    for (var i = 0; i < tdata.quantity; i++) {
        var particle = {
            position: { x: 0, y: 0 },
            shift: { x: 0, y: 0 },
//            position: { x: mouseArea.mouseX, y: mouseArea.mouseY },
//            shift: { x: mouseArea.mouseX, y: mouseArea.mouseY },
            size: 1,
            angle: 0,
            speed: 0.01+Math.random()*0.04,
            targetSize: 1,
            fillColor: '#' + (Math.random() * 0x404040 + 0xaaaaaa | 0).toString(16),
            orbit: tdata.radius*.5 + (tdata.radius* .5 * Math.random())
        };

        tdata.particles.push( particle );
    }
}

// eslint-disable-next-line @typescript-eslint/no-unused-vars
function setTargetSize(tdata, tsize) {
    for (var i = 0; i < tdata.particles.length; i++) {
        tdata.particles[i].targetSize = tsize;
    }
}

function isCleared(tdata) {
    for (var i = 0, len = tdata.particles.length; i < len; i++) {
        if (tdata.particles[i].size > 0.1) {
            return false;
        }
    }
    return true;

}

// eslint-disable-next-line @typescript-eslint/no-unused-vars
function loop(tdata) {

    var context = tdata.overlay.getContext('2d');
    if( tdata.mouseArea.pressed) {
        // Scale upward to the max scale
        tdata.radius_scale += ( tdata.radius_scale_max - tdata.radius_scale) * (0.02);
    }
    else {
        // Scale downward to the min scale
        tdata.radius_scale -= ( tdata.radius_scale - tdata.radius_scale_min) * (0.02);
    }

    tdata.radius_scale = Math.min( tdata.radius_scale, tdata.radius_scale_max);

    // Fade out the lines slowly by drawing a rectangle over the entire overlay
    if (tdata.fadeout && isCleared(tdata)) {
        context.clearRect(0, 0, tdata.overlay.width, tdata.overlay.height);
        return false;   // FC: return false so the controller can know it should stop calling loop()
    }

    // FC: using a string instead of Qt.rgba(), not sure it is compatible with Qt. Maybe pass this with tdata instead
    context.fillStyle = "rgba(255, 255, 255, 0.05)";
    context.fillRect(0, 0, tdata.overlay.width, tdata.overlay.height);

    for (var i = 0, len = tdata.particles.length; i < len; i++) {
        var particle = tdata.particles[i];

        var lp = { x: particle.position.x, y: particle.position.y };

        // Offset the angle to keep the spin going
        particle.angle += particle.speed;

        particle.shift.x += ( 0 - particle.shift.x) * (particle.speed);
        particle.shift.y += ( 0 - particle.shift.y) * (particle.speed);
        // Follow mouse with some lag
//        particle.shift.x += ( mouseArea.mouseX - particle.shift.x) * (particle.speed);
//        particle.shift.y += ( mouseArea.mouseY - particle.shift.y) * (particle.speed);

        // Apply position
        particle.position.x = particle.shift.x + Math.cos(i + particle.angle) * (particle.orbit*tdata.radius_scale);
        particle.position.y = particle.shift.y + Math.sin(i + particle.angle) * (particle.orbit*tdata.radius_scale);

        // Limit to screen bounds
        particle.position.x = Math.max( Math.min( particle.position.x, tdata.overlay.width), 0 );
        particle.position.y = Math.max( Math.min( particle.position.y, tdata.overlay.height), 0 );

        particle.size += ( particle.targetSize - particle.size ) * 0.05;

        // If we're at the target size, set a new one. Think of it like a regular day at work.
        if( particle.targetSize !== 0 && Math.round( particle.size ) == Math.round( particle.targetSize ) ) {
            particle.targetSize = 1 + Math.random() * 7;
        }

        context.beginPath();
        context.fillStyle = particle.fillColor;
        context.strokeStyle = particle.fillColor;
        context.lineWidth = particle.size;
        context.moveTo(lp.x, lp.y);
        context.lineTo(particle.position.x, particle.position.y);
        context.stroke();
        context.arc(particle.position.x, particle.position.y, particle.size/2, 0, Math.PI*2, true);
        context.fill();
    }

    return true;    // FC: return true so the controller can know it should continue calling loop()
}

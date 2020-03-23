
/**
 * adapted from http://hakim.se/experiments/
 */

function init() {

    if (overlay && overlay.getContext) {
        //var context = overlay.getContext('2d');
        createParticles();
    }
}

function createParticles() {
    overlay.particles = [];

    for (var i = 0; i < overlay.quantity; i++) {
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
            orbit: overlay.radius*.5 + (overlay.radius* .5 * Math.random())
        };

        overlay.particles.push( particle );
    }
}

function setTargetSize(tsize) {
    for (var i = 0; i < overlay.particles.length; i++) {
        overlay.particles[i].targetSize = tsize;
    }
}

function isCleared() {
    for (var i = 0, len = overlay.particles.length; i < len; i++) {
        if (overlay.particles[i].size > 0.1) {
            return false;
        }
    }
    return true;

}

function loop() {

    var context = overlay.getContext('2d');
    if( mouseArea.pressed) {
        // Scale upward to the max scale
        overlay.radius_scale += ( overlay.radius_scale_max - overlay.radius_scale) * (0.02);
    }
    else {
        // Scale downward to the min scale
        overlay.radius_scale -= ( overlay.radius_scale - overlay.radius_scale_min) * (0.02);
    }

    overlay.radius_scale = Math.min( overlay.radius_scale, overlay.radius_scale_max);

    // Fade out the lines slowly by drawing a rectangle over the entire overlay
    if (overlay.fadeout && isCleared()) {
        waittima.stop();
        context.clearRect(0, 0, overlay.width, overlay.height);
        return;
    }
    context.fillStyle = Qt.rgba(1,1,1,0.05);
    context.fillRect(0, 0, overlay.width, overlay.height);

    for (var i = 0, len = overlay.particles.length; i < len; i++) {
        var particle = overlay.particles[i];

        var lp = { x: particle.position.x, y: particle.position.y };

        // Offset the angle to keep the spin going
        particle.angle += particle.speed;

        particle.shift.x += ( 0 - particle.shift.x) * (particle.speed);
        particle.shift.y += ( 0 - particle.shift.y) * (particle.speed);
        // Follow mouse with some lag
//        particle.shift.x += ( mouseArea.mouseX - particle.shift.x) * (particle.speed);
//        particle.shift.y += ( mouseArea.mouseY - particle.shift.y) * (particle.speed);

        // Apply position
        particle.position.x = particle.shift.x + Math.cos(i + particle.angle) * (particle.orbit*overlay.radius_scale);
        particle.position.y = particle.shift.y + Math.sin(i + particle.angle) * (particle.orbit*overlay.radius_scale);

        // Limit to screen bounds
        particle.position.x = Math.max( Math.min( particle.position.x, overlay.width), 0 );
        particle.position.y = Math.max( Math.min( particle.position.y, overlay.height), 0 );

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
}





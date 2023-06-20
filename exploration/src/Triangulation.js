function getAngle(a, b, c) {
    // a: coordinate of p2
    // b: coordinate of target
    // c: coordinate of p1
    var ang = Math.atan2(c[1] - b[1], c[0] - b[0]) - Math.atan2(a[1] - b[1], a[0] - b[0]);
    if (ang < 0) {
        ang += 2 * Math.PI;
    }
    return ang;
}

function circle(p1, p2, angle) {
    // End points of the chord
    var x1 = p1[0], y1 = p1[1];
    var x2 = p2[0], y2 = p2[1];

    // Point on the line perpendicular to the chord
    // Note that this line also passes through the center of the circle
    var xm = (x1 + x2) / 2, ym = (y1 + y2) / 2;

    // Distance between p1 and p2
    var d_chord = Math.sqrt((x1 - x2) ** 2 + (y1 - y2) ** 2);

    // Distance between xm, ym and center of the circle (xc, yc)
    var d_perp = d_chord / (2 * Math.tan(angle / 2));

    var xc, yc, r;

    if (y1 === y2) {
        xc = xm;
        if (x1 < x2) {
            yc = ym - d_perp;
        } else {
            yc = ym + d_perp;
        }
    } else if (x1 === x2) {
        if (y1 < y2) {
            xc = xm + d_perp;
        } else {
            xc = xm - d_perp;
        }
        yc = ym;
    } else {
        // Slope of the line through the chord
        var slope = (y1 - y2) / (x1 - x2);
        // Slope of a line perpendicular to the chord
        var new_slope = -1 / slope;

        // Equation of line perpendicular to the chord: y-ym = new_slope(x-xm)
        // Distance between xm,ym and xc, yc: (yc-ym)^2 + (xc-xm)^2 = d_perp^2
        // Substituting from 1st to 2nd equation for y,
        //   we get: (new_slope^2+1)(xc-xm)^2 = d^2
        // Solve for xc:
        if (Math.sign(slope) === 1) {
            if (x1 < x2) {
                xc = d_perp / Math.sqrt(new_slope ** 2 + 1) + xm;
            } else {
                xc = -d_perp / Math.sqrt(new_slope ** 2 + 1) + xm;
            }
        } else {
            if (x1 < x2) {
                xc = -d_perp / Math.sqrt(new_slope ** 2 + 1) + xm;
            } else {
                xc = d_perp / Math.sqrt(new_slope ** 2 + 1) + xm;
            }
        }

        // Solve for yc:
        yc = new_slope * (xc - xm) + ym;
    }

    r = Math.sqrt((xc - p1[0]) ** 2 + (yc - p1[1]) ** 2);

    return [xc, yc, r];
}

function intersection(xc1, yc1, r1, xc2, yc2, r2, xc3, yc3, r3) {
    var d = Math.sqrt((xc2 - xc1) ** 2 + (yc2 - yc1) ** 2);

    // non intersecting
    if (d > r1 + r2) {
        return null;
    }
    // One circle within other
    if (d < Math.abs(r1 - r2)) {
        return null;
    }
    // coincident circles
    if (d === 0 && r1 === r2) {
        return null;
    } else {
        var a = (r1 ** 2 - r2 ** 2 + d ** 2) / (2 * d);
        var h = Math.sqrt(r1 ** 2 - a ** 2);

        var x_2 = xc1 + (a * (xc2 - xc1)) / d;
        var y_2 = yc1 + (a * (yc2 - yc1)) / d;

        var x_3 = x_2 + (h * (yc2 - yc1)) / d;
        var y_3 = y_2 - (h * (xc2 - xc1)) / d;

        var x_4 = x_2 - (h * (yc2 - yc1)) / d;
        var y_4 = y_2 + (h * (xc2 - xc1)) / d;

        // check satisfy equation of third circle
        if (
            Math.round((x_3 - xc3) ** 2 + (y_3 - yc3) ** 2, 5) ===
            Math.round(r3 ** 2, 5)
        ) {
            return [x_3, y_3];
        } else {
            return [x_4, y_4];
        }
    }
}
  
export async function triangulation(p1, p2, p3, alpha, beta) {
    var cir1, cir2, cir3, r1, r2, r3, x_target, xc1, xc2, xc3, y_target, yc1, yc2, yc3;
    [xc1, yc1, r1] = circle(p1, p2, 2 * alpha);
    [xc2, yc2, r2] = circle(p2, p3, 2 * beta);
    [xc3, yc3, r3] = circle(p1, p3, 2 * (alpha + beta));
    
    cir1 = new circle([xc1, yc1], {
        radius: r1,
        color: 'green',
        fill: false
    });
    cir2 = new circle([xc2, yc2], {
        radius: r2,
        color: 'red',
        fill: false
    });
    cir3 = new circle([xc3, yc3], {
        radius: r3,
        color: 'blue',
        fill: false
    });
/*
    gca().add_patch(cir1);
    gca().add_patch(cir2);
    gca().add_patch(cir3);

    plot(p1[0], p1[1], {
        color: 'black',
        marker: 'o'
    });
    plot(p2[0], p2[1], {
        color: 'black',
        marker: 'o'
    });
    plot(p3[0], p3[1], {
        color: 'black',
        marker: 'o'
    });
    plot(xc1, yc1, {
        color: 'orange',
        marker: 'o'
    });
    plot(xc2, yc2, {
        color: 'orange',
        marker: 'o'
    });
    plot(xc3, yc3, {
        color: 'orange',
        marker: 'o'
    });
*/
    [x_target, y_target] = intersection(xc1, yc1, r1, xc2, yc2, r2, xc3, yc3, r3);
    console.log(x_target, y_target);
    /*
    plot(x_target, y_target, {
        color: 'cyan',
        marker: 'o'
    });

    annotate("p1", [p1[0] + 0.1, p1[1] + 0.1]);
    annotate("p2", [p2[0] + 0.1, p2[1] + 0.1]);
    annotate("p3", [p3[0] + 0.1, p3[1] + 0.1]);
    annotate("target", [x_target + 0.1, y_target + 0.1]);

    show();
    */

}  
  
// test([1, 5], [7, 3], [0, 0], 1.1072, 1.5708);
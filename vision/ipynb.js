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
  
    var xc, yc;
  
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
        if (angle < 2 * Math.PI) {
          xc = (d_perp) / Math.sqrt(new_slope ** 2 + 1) + xm;
        } else {
          xc = -(d_perp) / Math.sqrt(new_slope ** 2 + 1) + xm;
        }
      } else {
        if (angle < 2 * Math.PI) {
          xc = -(d_perp) / Math.sqrt(new_slope ** 2 + 1) + xm;
        } else {
          xc = (d_perp) / Math.sqrt(new_slope ** 2 + 1) + xm;
        }
      }
  
      // Solve for yc:
      yc = new_slope * (xc - xm) + ym;
    }
  
    var r = Math.sqrt((xc - p1[0]) ** 2 + (yc - p1[1]) ** 2);
  
    return [xc, yc, r];
  }  
  

function intersection(xc1, yc1, r1, xc2, yc2, r2, xc3, yc3, r3) {
  var a, d, h, x_2, x_3, x_4, y_2, y_3, y_4;
  d = Math.sqrt(Math.pow(xc2 - xc1, 2) + Math.pow(yc2 - yc1, 2));

  if (d > r1 + r2) {
    return null;
  }

  if (d < Math.abs(r1 - r2)) {
    return null;
  }

  if (d === 0 && r1 === r2) {
    return null;
  } else {
    a = (Math.pow(r1, 2) - Math.pow(r2, 2) + Math.pow(d, 2)) / (2 * d);
    h = Math.sqrt(Math.pow(r1, 2) - Math.pow(a, 2));
    x_2 = xc1 + a * (xc2 - xc1) / d;
    y_2 = yc1 + a * (yc2 - yc1) / d;
    x_3 = x_2 + h * (yc2 - yc1) / d;
    y_3 = y_2 - h * (xc2 - xc1) / d;
    x_4 = x_2 - h * (yc2 - yc1) / d;
    y_4 = y_2 + h * (xc2 - xc1) / d;
  }

  if (Math.round(Math.pow(x_3 - xc3, 2) + Math.pow(y_3 - yc3, 2), 5) === Math.round(Math.pow(r3, 2), 5)) {
    return [x_3, y_3];
  } else {
    return [x_4, y_4];
  }
}
  
function test(p1, p2, p3, alpha, beta) {
    var cir1, cir2, cir3, r1, r2, r3, x_target, xc1, xc2, xc3, y_target, yc1, yc2, yc3;
    [xc1, yc1, r1] = circle(p1, p2, 2 * alpha);
    [xc2, yc2, r2] = circle(p2, p3, 2 * beta);
    [xc3, yc3, r3] = circle(p1, p3, 2 * (alpha + beta));
    cir1 = new circle([xc1, yc1], {
      "radius": r1,
      "color": "green",
      "fill": false
    });
    cir2 = new circle([xc2, yc2], {
      "radius": r2,
      "color": "red",
      "fill": false
    });
    cir3 = new circle([xc3, yc3], {
      "radius": r3,
      "color": "blue",
      "fill": false
    });
    /*
    gca().add_patch(cir1);
    gca().add_patch(cir2);
    gca().add_patch(cir3);
    plot(p1[0], p1[1], {
      "color": "black",
      "marker": "o"
    });
    plot(p2[0], p2[1], {
      "color": "black",
      "marker": "o"
    });
    plot(p3[0], p3[1], {
      "color": "black",
      "marker": "o"
    });
    plot(xc1, yc1, {
      "color": "orange",
      "marker": "o"
    });
    plot(xc2, yc2, {
      "color": "orange",
      "marker": "o"
    });
    plot(xc3, yc3, {
      "color": "orange",
      "marker": "o"
    });
    */
    [x_target, y_target] = intersection(xc1, yc1, r1, xc2, yc2, r2, xc3, yc3, r3);
    /*
    plot(x_target, y_target, {
      "color": "cyan",
      "marker": "o"
    });
    plt.annotate("p1", [p1[0] + 0.1, p1[1] + 0.1]);
    plt.annotate("p2", [p2[0] + 0.1, p2[1] + 0.1]);
    plt.annotate("p3", [p3[0] + 0.1, p3[1] + 0.1]);
    plt.annotate("target", [x_target + 0.1, y_target + 0.1]);
    show();
    */
   console.log([x_target, y_target]);

  }
  
  
test([0, 0], [1, 5], [7, 3], 1.8926, 2.5536);
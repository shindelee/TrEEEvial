import { Odometry } from "../src/dead_reckoning.js";
import prompt from 'prompt-sync';

var parent_x;
 var parent_y;
//  var seq_no = 0;
 var directions = "";
 var buffer = "";
 var x_accurate = 0;
 var y_accurate = 0;
 var x_coord = 0;
 var y_coord = 0;
 var heading = 0;
 var server;

parent_x = 0;
 parent_y = -0.1;

while (1) {
  let x = prompt("input x");
  let y = prompt("input y");
var coords_odo = await Odometry((parseInt(x)) , parseInt(y));
                heading = heading + coords_odo[2];
                if (heading < 0 ) {
                    heading = heading + 360;
                }
                if (heading > 360) {
                    heading = heading - 360;
                }
                console.log(coords_odo);

                var hyp = Math.sqrt(Math.pow(coords_odo[0], 2) + Math.pow(coords_odo[1], 2));
                    console.log(hyp*100*Math.sin(heading*Math.PI/180));
                    console.log(hyp*100*Math.cos(heading*Math.PI/180));
                    x_accurate = hyp*100*Math.sin(heading*Math.PI/180) + x_accurate;
                    y_accurate = hyp*100*Math.cos(heading*Math.PI/180) + y_accurate;

                    console.log("x_accurate = " + x_accurate);
                console.log("y_accurate = " + y_accurate);

                x_coord = Math.floor(x_accurate/3);
                y_coord = Math.floor(y_accurate/3);
                
                console.log("x-coord = " + x_coord + " , y-coord = " +y_coord);
        
                //directions = await Tremaux(x_coord,y_coord, parent_x, parent_y, right_wall, left_wall,front_wall, heading);
                parent_x = x_coord;
                parent_y = y_coord;

                parent_x =0;
                parent_y = 0;
            
}
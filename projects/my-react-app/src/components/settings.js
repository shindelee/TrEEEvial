let form = document.querySelector("#settings");
let size = document.querySelector("#size");
let rowsCols = document.querySelector("#number");
let complete = document.querySelector(".complete");
let replay = document.querySelector(".replay");
let close = document.querySelector(".close");

let newMaze;

function wait(ms){
  var start = new Date().getTime();
  var end = start;
  while(end < start + ms) {
    end = new Date().getTime();
 }
}

form.addEventListener("submit", generateMaze);
document.addEventListener("keydown", move);
replay.addEventListener("click", () => {
  location.reload();
});

close.addEventListener("click", () => {
  complete.style.display = "none";
});

function generateMaze(e) {
  e.preventDefault();

  if (rowsCols.value == "" || size.value == "") {
    return alert("Please enter all fields");
  }

  let mazeSize = size.value;
  let number = rowsCols.value;
  if (mazeSize > 600 || number > 50) {
    alert("Maze too large!");
    return;
  }

  form.style.display = "none";

  newMaze = new Maze(mazeSize, number, number);
  newMaze.setup();
  newMaze.draw();
  console.log("We finished drawing the maze.");
}
i=0;

function move() {
  console.log('We reached the arrow key game part.');
  
  if (!generationComplete) return;
  console.log('Generation is complete.');
  // shortest_path = [[0, 1], [0, 2], [0, 3], [0, 4], [0, 5], [1, 5], [2, 5], [3, 5], [4, 5], [5, 5], [6, 5], [6, 6], [6, 7], [7, 7]];

    if (!generationComplete) return;
    shortest_path = [[ 0, 1 ], [ 0, 2 ], [ 0, 3 ], [ 0, 4 ], [ 0, 5 ], [ 0, 6 ], [ 0, 7 ], [ 0, 8 ], [ 0, 9 ], [ 0, 10 ], [ 0, 11 ], [ 0, 12 ], [ 0, 13 ], [ 1, 13 ],  [ 2, 13 ], [ 3, 13 ], [ 4, 13 ], [ 5, 13 ], [ 6, 13 ], [ 7, 13 ], [ 7, 14 ], [ 7, 15 ], [ 8, 15 ], [ 9, 15 ], [ 10, 15 ], [ 11, 15 ], [ 12, 15 ], [ 13, 15 ], [ 14, 15 ], [ 15, 15 ]];
      let next = newMaze.grid[shortest_path[i][0]][shortest_path[i][1]];
      i++;
      current.unhighlight(newMaze.columns);
      current = next;
      //newMaze.draw();
      current.highlight(newMaze.columns);
    // not required if goal is in bottom right
    if (current.goal) complete.style.display = "block";
    console.log(i);

  /*

  switch (key) {
    case "ArrowUp":
      if (!current.walls.topWall) {
        let next = newMaze.grid[row - 1][col];
        current.unhighlight(newMaze.columns);
        current = next;
        //newMaze.draw();
        current.highlight(newMaze.columns);
        // not required if goal is in bottom right
        if (current.goal) complete.style.display = "block";
      }
      break;

    case "ArrowRight":
      if (!current.walls.rightWall) {
        let next = newMaze.grid[row][col + 1];
        current.unhighlight(newMaze.columns);
        current = next;
        //newMaze.draw();
        current.highlight(newMaze.columns);
        if (current.goal) complete.style.display = "block";
      }
      break;

    case "ArrowDown":
      if (!current.walls.bottomWall) {
        let next = newMaze.grid[row + 1][col];
        current.unhighlight(newMaze.columns);
        current = next;
        //newMaze.draw();
        //console.log(current);
        current.highlight(newMaze.columns);
        if (current.goal) complete.style.display = "block";
      }
      break;

    case "ArrowLeft":
      if (!current.walls.leftWall) {
        let next = newMaze.grid[row][col - 1];
        current.unhighlight(newMaze.columns);
        current = next;
        //newMaze.draw();
        current.highlight(newMaze.columns);
        // not required if goal is in bottom right
        if (current.goal) complete.style.display = "block";
      }
      break;
  }

  */
}

/*
function tracing(){
  while(!current.goal){
    move();
  }
}
*/
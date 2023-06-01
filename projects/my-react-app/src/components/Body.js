import React from 'react';

function Body(){
    return(
        <div>
            <body>
          
                <h1 className="title">Finding Best Path</h1>
      
                <div className="complete">
                    <h3>Maze Completed.</h3>
                    <button className="replay" id="submit">Play again</button>
                    <button className="close" id="submit">Close</button>
                </div>
          
                <form id="settings" action="">
                  <p>Maze Size</p>
                  <input id="size" type="number" value="500"/><br />
                  <p>Rows</p>
                  <input id="number" type="number" value="8" /><br />
                  <p>Columns</p>
                  <input id="number" type="number" value="12" /><br />
                  <input id="submit" type="submit" value="Generate Maze" />
                </form>
  
                <canvas className="maze"></canvas>
            </body> 
        </div>
    )
}

export default Body;
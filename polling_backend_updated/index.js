import { newMazeState } from "./newMazeState.js";
import { aStarAlgorithm } from "./astar.js";
import express from 'express';
import cors from 'cors';


const PORT = process.env.PORT || 3001;
const app = express();

app.use(cors({
    origin: '*'
}));

//app.use(express.static(path.resolve(__dirname, './updated2.0/build')));

app.use(cors({
    methods: ['GET','POST','DELETE','UPDATE','PUT','PATCH']
}));

app.get("/initial", (req, res) => {
    setTimeout(()=>{
        res.json(
            {
            "tableData33":[
            [0,2,0,0,1,0,0,0,0],
            [0,0,1,1,1,1,0,1,0],
            [0,0,0,0,0,1,0,1,0],
            [1,0,1,0,1,1,0,1,0],
            [1,0,1,0,1,1,0,1,0],
            [1,0,1,0,0,0,0,1,0],
            [1,0,1,1,1,1,0,1,0],
            [1,0,0,0,0,0,0,0,0],
            [1,1,1,1,1,0,0,0,0]
            ]
        })
    }, 1000);
});

app.get("/tableData33", (req, res) => {
    setTimeout(()=>{
        res.json(
            {
            "tableData33":[
            [2,0,0,0,1,0,0,0,0],
            [0,1,1,1,1,1,0,1,0],
            [0,0,0,0,0,1,0,1,0],
            [1,0,1,0,1,1,0,1,0],
            [1,0,1,0,1,1,0,1,0],
            [1,0,1,0,0,0,0,1,0],
            [1,0,1,1,1,1,0,1,0],
            [1,0,0,0,0,0,0,0,0],
            [1,1,1,1,1,0,0,0,0]
            ]
            
        })
    }, 1000);
});

app.get("/shortestPath", (req, res) => {
    var coordinates = aStarAlgorithm(20, 20, 150, 155, State);
    for(let k = 0; k < coordinates.length; k++){
        State[coordinates[k][0]][coordinates[k][1]] = 4;
    }
    setTimeout(()=>{
        res.json(
            {
            "shortestPath": State
        })
    }, 1000);
});


// Prerequisites for newMazeState polling.
// 366x225 maze for cross junction large case.
const State = [];
for(let j = 0; j < 366; j++){
    let row = []
    for(let j = 0; j < 225; j++){
        row.push(1);
    }
    State.push(row);
}

const widenBy = 6;

app.get("/pollServer", (req, res) => {
    var d = new Date();
    

    // Instructions from actual maze and Tremaux.
    newMazeState(State, [20,20], [20, 60], widenBy);
    newMazeState(State, [20,60], [57, 60], widenBy);
    // Backtracking shouldn't update map. Rover position needs to be handled.
    newMazeState(State, [20,20], [92, 20], widenBy);
    newMazeState(State, [92,20], [92, 98], widenBy);
    newMazeState(State, [92,98], [20, 98], widenBy);
    newMazeState(State, [20,98], [20, 205], widenBy);
    newMazeState(State, [20,205], [92, 205], widenBy);
    newMazeState(State, [92,205], [280, 205], widenBy);
    newMazeState(State, [280,205], [311, 205], widenBy);
    newMazeState(State, [311,205], [346, 205], widenBy);
    newMazeState(State, [346,205], [346, 20], widenBy);
    newMazeState(State, [346,20], [290, 20], widenBy);
    // Backtrack.
    newMazeState(State, [311,205], [238, 60], widenBy);
    newMazeState(State, [238,60], [228, 20], widenBy);
    newMazeState(State, [228,20], [248, 20], widenBy);
    // Backtrack.
    newMazeState(State, [228,20], [208, 20], widenBy);
    // Backtrack.
    newMazeState(State, [238,60], [92, 98], widenBy);
    // Backtrack.
    newMazeState(State, [280,205], [208, 105], widenBy);
    newMazeState(State, [208,105], [150, 155], widenBy);
    newMazeState(State, [150,155], [130, 190], widenBy);
    newMazeState(State, [130,190], [130, 155], widenBy);
    newMazeState(State, [130,155], [150, 155], widenBy);
    var myArray = [];
    for(let i = 0; i < 366; i++){
        myArray.push(State[i]);
    }
    const json_res = {
    "time" : myArray
    };
    res.send(json_res);
    }); 

/*
app.get('*', (req, res) => {
    res.sendFile(path.resolve(__dirname, './updated2.0/build', 'index.html'));
});
*/

app.listen(PORT, () => {
 console.log(`Server listening on ${PORT}`);
});

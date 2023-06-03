import React from "react";
import TableComp from './TableComp';

function App() {
 let tableData33 =[
 [2,0,0,0,0,0,0,0,0],
 [0,1,1,1,1,1,0,1,0],
 [0,0,0,0,0,0,0,1,0],
 [0,0,1,0,1,1,0,1,0],
 [0,0,1,0,1,1,0,1,0],
 [0,0,1,0,0,0,0,1,0],
 [1,0,1,1,1,0,0,1,0],
 [1,0,0,0,0,0,0,0,0],
 [1,1,1,1,1,0,0,0,0]
 ];
 return (
 <div className="App">
 <TableComp td = {tableData33}/>
 </div>
 );
}
export default App;
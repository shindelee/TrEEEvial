import React, { useState } from "react";
import TableComp from './TableComp';

function App() {

    const [tableData33, updateTable33] = useState([[]]);

    React.useEffect(() => {
        ///See CORS
        fetch("http://localhost:3001/initial/")
        .then((res) => res.json())
        .then((data) => updateTable33(data.tableData33))
        .catch((err) => alert(err)
        );
    }, [updateTable33]);

    const handleClick = (updateMethod) => {
        fetch("http://localhost:3001/tableData33/")
        .then((res) => res.json())
        .then((data) => updateMethod(data.tableData33))
        .catch((err) => alert(err)
        );
        };

    const handleClick2 = (updateMethod) => {
        fetch("http://localhost:3001/shortestPath/")
        .then((res) => res.json())
        .then((data) => updateMethod(data.shortestPath))
        .catch((err) => alert(err)
        );
        };

    return (
        <div className="App">
            <TableComp td = {tableData33}/>
            <button onClick={()=>handleClick(updateTable33)}>New maze state</button>
            <button onClick={()=>handleClick2(updateTable33)}>Display shortest path</button>
        </div>
    );
}
export default App;
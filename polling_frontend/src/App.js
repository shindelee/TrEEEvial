import React, { useState } from "react";
import TableComp from './TableComp';
import ReactPolling from "react-polling/lib/ReactPolling";

function App() {

    const [tableData33, updateTable33] = useState([[]]);

    const fetchData = () => {
        return fetch("http://localhost:3001/pollServer/");
        }

    const pollingSuccess = (jsonResponse) => {
        updateTable33(jsonResponse.time);
        return true;
    }

    const pollingFailure = () => {
        //alert('Polling failed');
        //return true;
    }

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
            <ReactPolling
                url={'http://localhost:3001/pollServer/'}
                interval= {1000} // in milliseconds(ms)
                retryCount={3} // this is optional
                onSuccess = {pollingSuccess}
                onFailure= {pollingFailure}
                promise={fetchData} // custom api calling function that should return a promise
                render={({ startPolling, stopPolling, isPolling }) => {
                // return <div>{tableData33}<br/><br/></div>;
                }}
            />
            <TableComp td = {tableData33}/>
            <button onClick={()=>handleClick(updateTable33)}>New maze state</button>
            <button onClick={()=>handleClick2(updateTable33)}>Display shortest path</button>
        </div>
    );
}
export default App;
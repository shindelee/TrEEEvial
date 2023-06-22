import '../../App.css';
import React, { useState } from "react";
import TableComp from '../../TableComp';
import styled from 'styled-components';
import ReactPolling from "react-polling/lib/ReactPolling";

function Home() {

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
    
    const Button1 = styled.button`
        background-color: #9c27b0;
        color: white;
        padding: 5px 10px;
        border-radius: 5px;
        outline: 0;
        margin: 10px 10px;
        cursor: pointer;
        box-shadow: 0px 2px 2px lightgray;
        &: hover{
            background-color: #ba68c8;
        }
    `

    return (
        <div className="Home">
            <TableComp td = {tableData33}/>
            <Button1 onClick={()=>handleClick2(updateTable33)}>
                Display shortest path
            </Button1>
            <ReactPolling
                url={'http://localhost:3001/pollServer/'}
                interval= {1000} // in milliseconds(ms)
                retryCount={3} // this is optional
                onSuccess = {pollingSuccess}
                onFailure= {pollingFailure}
                promise={fetchData} // custom api calling function that should return a promise
                render={({ startPolling, stopPolling, isPolling }) => {
                return isPolling ? (
                    <Button1 onClick={stopPolling}>Stop Polling</Button1>
                ) : null;
                }}
            />
        </div>
    );
}

export default Home;
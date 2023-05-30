//React props are used to pass parameters 
//to reach components
//Read more about React Props here
//https://www.w3schools.com/react/react_props.asp
function Table33(props) { 
    return (
        <div className="Table33"> 
            <table>
                <tr> 
                    <th>Name</th> 
                    <th>Age</th> 
                    <th>Gender</th>
                </tr> 
                <tr>
                    <td>{props.td[0][0]}</td>
                    <td>{props.td[0][1]}</td>
                    <td>{props.td[0][2]}</td> 
                </tr>
                <tr> 
                    <td>{props.td[1][0]}</td>
                    <td>{props.td[1][1]}</td>
                    <td>{props.td[1][2]}</td> 
                </tr>
                <tr> 
                    <td>{props.td[2][0]}</td> 
                    <td>{props.td[2][1]}</td> 
                    <td>{props.td[2][2]}</td>
                </tr> 
            </table>
        </div> 
    );
}
export default Table33;
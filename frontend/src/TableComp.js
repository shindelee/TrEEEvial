import './TableComp.css'

function TableComp(props) {

    function WallOrNot(x){
        if(x === 1){
            return <td className='maze_wall_cell'>
            </td>
        }
        else if(x === 0){
            return <td className='maze_empty_cell'>
            </td>
        }
        else {
            return <td className='maze_rover_cell'>
            </td>
        }
    }

    //Inner loop
     function TableCols(props){
        const cols = props.tr.map(
            WallOrNot
        )
        return cols;
     }
    //Map each row to its corresponding HTML
    function TableRows(props){
     const rows = props.td.map(
        (row) =>
        <tr>
            <TableCols tr = {row}/>
        </tr>
     )
     return rows;
    }
    
     return (
     <div className="TableComp">
     <table className="maze_display">
     <TableRows td={props.td}/>
     </table>
     </div>
     );
    
     }
     export default TableComp;
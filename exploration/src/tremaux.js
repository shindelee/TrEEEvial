function Tremaux (){
    visited[current_pos] += 1;
    wall.Front = rec.Front;
    wall.Left = rec.Left;
    wall.Right = rec.Right;
    last_visited = current_pos;
    
    if (Front === 0 && Left === 1 && Right === 1){
        msg = forward;
        rover.send(msg);
    }

    else if (Front === 0 && Left === 0 && Right === 1){
        if (visited[Front] < visited[Left]){
            msg = forward;
            rover.send(msg);
        }
        else{
            msg = go_left;
            rover.send(msg);
        }
    }

    else if (Front === 0 && Left === 1 && Right === 0){
        if (visited[Front] < visited[Right]){
            msg = forward;
            rover.send(msg);
        }
        else{
            msg = go_right;
            rover.send(msg);
        }
    }

    else if (Front === 0 && Left === 0 && Right === 0){
        if (Math.min(visited[Front], visited[Left], visited[Right]) === visited[Front]){
            msg = forward;
            rover.send(msg);
        }
        else if (Math.min(visited[Front], visited[Left], visited[Right]) === visited[Left]){
            msg = go_left;
            rover.send(msg);
        }
        else{
            msg = go_right;
            rover.send(msg);
        }
    }

    else if (Front === 1 && Left === 0 && Right === 0){
        if (visited[Left] < visited[Right]){
            msg = go_left;
            rover.send(msg);
        }
        else{
            msg = go_right;
            rover.send(msg);
        }
    }

    else if (Front === 1 && Left === 1 && Right === 0){
        msg = go_right;
        rover.send(msg);
    }

    else if (Front === 1 && Left === 0 && Right === 1){
        msg = go_left;
        rover.send(msg);
    }

    else{
        msg = backtrack;
        rover.send(msg);
    }
}

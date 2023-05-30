class Node {
    constructor(row, col, value) {
      this.id = row.toString() + '-' + col.toString();
      this.row = row; // "y" location of grid
      this.col = col; // "x" location of grid
      this.value = value;
      this.distanceFromStart = Infinity; // G
      this.estimatedDistanceToEnd = Infinity; // F = G + H
      this.cameFrom = null; // only startNode does not have this property
    }
  }
  
  function aStarAlgorithm(startRow, startCol, endRow, endCol, graph) {
    const nodes = initializeNodes(graph);
    const startNode = nodes[startRow][startCol];
    const endNode = nodes[endRow][endCol];
  
    startNode.distanceFromStart = 0;
    // H (heuristic)
    startNode.estimatedDistanceToEnd = calculateManhattanDistance(startNode, endNode);
    
    const nodesToVisit = new MinHeap([startNode]);
  
    while (!nodesToVisit.isEmpty()){
      // visit nodes with least F
      const currentMinDistanceNode = nodesToVisit.remove();
      // reached the endNode
      if (currentMinDistanceNode === endNode) break;
  
      const neighbors = getNeighboringNodes(currentMinDistanceNode, nodes);
      for (const neighbor of neighbors) {
        // skip wall
        if (neighbor.value == 1) continue;
        const tentativeDistanceToNeighbor = currentMinDistanceNode.distanceFromStart + 1;
  
        // skip nodes with large G
        if (tentativeDistanceToNeighbor >= neighbor.distanceFromStart) continue;
  
        neighbor.cameFrom = currentMinDistanceNode;
        neighbor.distanceFromStart = tentativeDistanceToNeighbor; // G
        neighbor.estimatedDistanceToEnd = tentativeDistanceToNeighbor + calculateManhattanDistance(neighbor, endNode); // F = G + H
        
        // heap modification
        if (!nodesToVisit.containsNode(neighbor)){
          nodesToVisit.insert(neighbor);
        }
        else {
          nodesToVisit.update(neighbor);
        }
      }
      
    }
  
    return reconstructPath(endNode);
  }
  
  // Same structure as input but a 2-D array with Nodes objects added inside it 
  // instead of 1's and 0's
  function initializeNodes(graph){
    const nodes = []
  
    for (const [i, row] of graph.entries()) {
      nodes.push([]);
      for (const [j, value] of row.entries()) {
        const node = new Node(i, j, value);
        nodes[i].push(node);
      }
    }
  
    return nodes;
  }
  
  // H (heuristic -> Manhattan Distance)
  // Chose Manhattan Distance due to 4 directions movement
  function calculateManhattanDistance(currentNode, endNode){
    const currentRow = currentNode.row;
    const currentCol = currentNode.col;
    const endRow = endNode.row;
    const endCol = endNode.col;
  
    return Math.abs(currentRow - endRow) + Math.abs(currentCol - endCol);
  }
  
  function getNeighboringNodes(node, nodes){
    const neighbors = [];
  
    const numRows = nodes.length;
    const numCols = nodes[0].length;
  
    const row = node.row;
    const col = node.col;
    
    // 4 positions of neighbours
    if (row < numRows - 1){ 
      // DOWN
      neighbors.push(nodes[row + 1][col]);
    }
  
    if (row > 0){
      // UP 
      neighbors.push(nodes[row - 1][col]);
    }
  
    if (col < numCols - 1){
      // RIGHT
      neighbors.push(nodes[row][col + 1]);
    }
  
    if (col > 0){
      // LEFT
      neighbors.push(nodes[row][col - 1]);
    }
  
    return neighbors;
  }
  
  function reconstructPath(endNode){
    // no shortest path
    if(endNode.cameFrom == null){
      return [];
    }
  
    // shortest path exits
    let currentNode = endNode;
    const path = [];
  
    // tracking back to where each node comes from
    while(currentNode != null){
      path.push([currentNode.row, currentNode.col]);
      currentNode = currentNode.cameFrom;
    }
  
    // since we add the endNode first, and the second last node after, we need to reverse the order of the nodes
    path.reverse();
    return path;
  }
  
  class MinHeap {
    constructor(array) {
      // specific feature
      // Holds the position in the heap that each node is at
      this.nodePositionsInHeap = array.reduce((obj, node, i) => {
        obj[node.id] = i;
        return obj;
      }, {});
  
      this.heap = this.buildHeap(array);
    }
  
    isEmpty() {
      return this.heap.length == 0;
    }
  
    // O(n) time | O(1) space
    buildHeap(array) {
      const firstParentIdx = Math.floor((array.length - 2) / 2);
      for (let currentIdx = firstParentIdx; currentIdx >= 0; currentIdx --){
        this.siftDown(currentIdx, array.length - 1, array);
      }
  
      return array;
    }
  
    // O(log n) time | O(1) space
    siftDown(currentIdx, endIdx, heap) {
      let childOneIdx = currentIdx * 2 + 1;
      while(childOneIdx <= endIdx) {
        const childTwoIdx = currentIdx * 2 + 2 <= endIdx ? currentIdx * 2 + 2 : -1;
        let idxToSwap;
        if (childTwoIdx !== -1 && heap[childTwoIdx].estimatedDistanceToEnd < heap[childOneIdx].estimatedDistanceToEnd){
          idxToSwap = childTwoIdx;
        }
        else {
          idxToSwap = childOneIdx;
        }
        if (heap[idxToSwap].estimatedDistanceToEnd < heap[currentIdx].estimatedDistanceToEnd) {
          this.swap(currentIdx, idxToSwap, heap);
          currentIdx = idxToSwap;
          childOneIdx = currentIdx * 2 + 1;
        }
        else {
          return;
        }
      }
    }
  
    // O(log n) time | O(1) space
    siftUp(currentIdx, heap){
      let parentIdx = Math.floor((currentIdx - 1) / 2);
      while (currentIdx > 0 && heap[currentIdx].estimatedDistanceToEnd < heap[parentIdx].estimatedDistanceToEnd) {
        this.swap(currentIdx, parentIdx, heap);
        currentIdx = parentIdx;
        parentIdx = Math.floor((currentIdx - 1) / 2);
      }
    }
    
    // O(log n) time | O(1) space
    remove() {
      if(this.isEmpty()) return;
  
      this.swap(0, this.heap.length - 1, this.heap);
      const node = this.heap.pop();
      delete this.nodePositionsInHeap[node.id];
      this.siftDown(0, this.heap.length - 1, this.heap);
      return node;
    }
  
    // O(log n) time | O(1) space
    insert(node) {
      this.heap.push(node);
      this.nodePositionsInHeap[node.id] = this.heap.length - 1;
      this.siftUp(this.heap.length - 1, this.heap);
    }
  
    swap(i, j, heap) {
      this.nodePositionsInHeap[this.heap[i].id] = j;
      this.nodePositionsInHeap[this.heap[j].id] = i;
      const temp = heap[j];
      heap[j] = heap[i];
      heap[i] = temp;
    }
  
    containsNode(node) {
      return node.id in this.nodePositionsInHeap;
    }
    
    update(node) {
      this.siftUp(this.nodePositionsInHeap[node.id], this.heap);
    }
    
  }


// Test Case below: (Can exclude this part)
startRow = 0;
startCol = 1;
endRow = 4;
endCol = 3;
graph = [
    [0, 0, 0, 0, 0],
    [0, 1, 1, 1, 0],
    [0, 0, 0, 0, 0],
    [1, 0, 1, 1, 1],
    [0, 0, 0, 0, 0],
]

console.log(aStarAlgorithm(startRow, startCol, endRow, endCol, graph));

// The path should be:
//    [
//        [0, 1], [0, 0], [1, 0], [2, 0], [2, 1], [3, 1], [4, 1], [4, 2], [4, 3]
//    ]



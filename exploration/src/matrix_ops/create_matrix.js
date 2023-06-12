export function create_matrix(m, n) {
    const matrix = [];
    
    for (let i = 0; i < m; i++) {
      const row = new Array(n).fill(0);
      matrix.push(row);
    }
    
    return matrix;
  }
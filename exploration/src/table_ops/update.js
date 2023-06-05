export const updateName = async (new_val) => {
    const aws = require('aws-sdk');
    const docClient = new aws.DynamoDB.DocumentClient();
  
    const params = {
      TableName: 'Node_Information',
      Key: {
        x: X,
        y: Y
      },
      UpdateExpression: 'SET Visited = :r',
      ExpressionAttributeValues: {
        ':r': new_val,
      },
    };
    await docClient.update(params).promise();
  }
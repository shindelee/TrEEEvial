import { DynamoDBClient } from "@aws-sdk/client-dynamodb";
import { UpdateCommand, DynamoDBDocumentClient } from "@aws-sdk/lib-dynamodb";

const client = new DynamoDBClient({
<<<<<<< HEAD
  region: "x",
  endpoint: "http://localhost:8000",
  credentials: {
    accessKeyId: "abcd",
    secretAccessKey: "1234",
  },
=======
  region: "us-east-1",
>>>>>>> 9ea4e4ec4e24f9e1ee23b0ac96d5a04bc92a2008
});

const docClient = DynamoDBDocumentClient.from(client);

export const update_visited = async (X,Y, update) => {
  const command = new UpdateCommand({
    TableName: "Node_Information",
    Key: {
      x : X,
      y :Y
    },
    UpdateExpression: 'SET path_count = :r',
    ExpressionAttributeValues: {
      ':r': update,
    },
    ReturnValues: "UPDATED_NEW",
  });

  const response = await docClient.send(command);
  // console.log(response);
  return response;
};
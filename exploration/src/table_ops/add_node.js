import { DynamoDBClient } from "@aws-sdk/client-dynamodb";
import { PutCommand, DynamoDBDocumentClient } from "@aws-sdk/lib-dynamodb";

const client = new DynamoDBClient({
<<<<<<< HEAD
  region: "x",
    endpoint: "http://localhost:8000",
    credentials: {
      accessKeyId: "abcd",
      secretAccessKey: "1234",
=======
  region: "us-east-1",
>>>>>>> 9ea4e4ec4e24f9e1ee23b0ac96d5a04bc92a2008
},
);
const docClient = DynamoDBDocumentClient.from(client);

export const add_node = async (X,Y, Parent_x, Parent_y, unexplored) => {
  const command = new PutCommand({
    TableName: "Node_Information",
    Item: {
      x : X,
      y : Y,
      parent : {x : Parent_x, y : Parent_y},
      path_count : unexplored,
    },
  });

  const response = await docClient.send(command);
  // console.log(response);
  return response;
};

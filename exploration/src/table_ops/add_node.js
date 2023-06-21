import { DynamoDBClient } from "@aws-sdk/client-dynamodb";
import { PutCommand, DynamoDBDocumentClient } from "@aws-sdk/lib-dynamodb";

const client = new DynamoDBClient({
  // region: "eu-north-1",
  region: "x",
  endpoint: "http://localhost:8000",
  credentials: {
  accessKeyId: "abcd",
  secretAccessKey: "1234",
  },
});

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

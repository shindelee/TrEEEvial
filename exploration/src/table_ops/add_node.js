import { DynamoDBClient } from "@aws-sdk/client-dynamodb";
import { PutCommand, DynamoDBDocumentClient } from "@aws-sdk/lib-dynamodb";

const client = new DynamoDBClient({
  region: "x",
    endpoint: "http://localhost:8000",
    credentials: {
      accessKeyId: "abcd",
      secretAccessKey: "1234",
    },
});
const docClient = DynamoDBDocumentClient.from(client);

export const add_node = async (X,Y, Parent_x, Parent_y) => {
  const command = new PutCommand({
    TableName: "Node_Information",
    Item: {
      x : X,
      y : Y,
      type: 1,
      parent_x : Parent_x,
      parent_y : Parent_y,
      visited : 0
    },
  });

  const response = await docClient.send(command);
  console.log(response);
  return response;
};

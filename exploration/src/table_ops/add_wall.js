import { DynamoDBClient } from "@aws-sdk/client-dynamodb";
import { PutCommand, DynamoDBDocumentClient } from "@aws-sdk/lib-dynamodb";

const client = new DynamoDBClient({
  region: "us-east-1"
});
const docClient = DynamoDBDocumentClient.from(client);

export const add_wall = async (X,Y) => {
  const command = new PutCommand({
    TableName: "Node_Information",
    Item: {
      x : X,
      y : Y,
      type: 1, //wall
    },
  });

  const response = await docClient.send(command);
  console.log(response);
  return response;
};
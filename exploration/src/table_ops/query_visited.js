import { DynamoDBClient } from "@aws-sdk/client-dynamodb";
import { QueryCommand, DynamoDBDocumentClient } from "@aws-sdk/lib-dynamodb";

const client = new DynamoDBClient({});
const docClient = DynamoDBDocumentClient.from(client);

export const queryv = async (X, Y) => {
  const command = new QueryCommand({
    TableName: "Node_Information",
    KeyConditionExpression:
      "x = :visited_x AND y = :visited_y",
    ExpressionAttributeValues: {
      ":visited_x": "X",
      ":visited_y": "Y",
    },
    ConsistentRead: true,
  });

  const response = await docClient.send(command);
  console.log(response.visited);
  return response.visited;
};


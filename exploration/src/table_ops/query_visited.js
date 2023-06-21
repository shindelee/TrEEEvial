import { DynamoDBClient } from "@aws-sdk/client-dynamodb";
import { DynamoDBDocumentClient, GetCommand } from "@aws-sdk/lib-dynamodb";

const client = new DynamoDBClient({
  region: "eu-north-1",
});

const docClient = DynamoDBDocumentClient.from(client);

export const query_visited = async (X,Y) => {
  const command = new GetCommand({
    TableName: "Node_Information",
    Key: {
      x: X,
      y: Y,
    }
  });

  const response = await docClient.send(command);
  // console.log(response);
  return response.Item.path_count;
};


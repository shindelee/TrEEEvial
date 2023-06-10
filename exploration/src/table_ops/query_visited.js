import { DynamoDBClient } from "@aws-sdk/client-dynamodb";
import { QueryCommand, DynamoDBDocumentClient } from "@aws-sdk/lib-dynamodb";

const client = new DynamoDBClient({
  region: "x",
    endpoint: "http://localhost:8000",
    credentials: {
      accessKeyId: "abcd",
      secretAccessKey: "1234",
    },
});
const docClient = DynamoDBDocumentClient.from(client);

export const query_visited = async (X, Y) => {
  const command = new QueryCommand({
    TableName: "Node_Information",
    KeyConditionExpression:
      "x = :visited_x AND y = :visited_y",
    ExpressionAttributeValues: {
      ":visited_x": X,
      ":visited_y": Y,
    },
    ProjectionExpression: "visited",

    ConsistentRead: true,
  });

  const response = await docClient.send(command);
    // console.log("number of visitations: " + response);
    var return_vals = [];
    response.Items.forEach(function (result) {
      console.log(`${result.visited_count}\n`);
      return_vals.push(result.visited_count);
    });

    return parseInt(return_vals);
    
    
    
};


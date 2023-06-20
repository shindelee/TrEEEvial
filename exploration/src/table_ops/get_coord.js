//check if coordinate is in table

import { DynamoDBClient } from "@aws-sdk/client-dynamodb";
import { DynamoDBDocumentClient, GetCommand } from "@aws-sdk/lib-dynamodb";

const client = new DynamoDBClient({

});
const docClient = DynamoDBDocumentClient.from(client);

export const get_coord = async (X,Y) => {
  const command = new GetCommand({
    TableName: "Node_Information",
    Key: {
      x: X,
      y: Y,
    }
  });

  const response = await docClient.send(command);
  console.log(response);
  if (response.Item) {
    return true;
  }

  else {
    return false;
  }
};

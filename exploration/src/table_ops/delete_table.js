import { DeleteTableCommand, DynamoDBClient } from "@aws-sdk/client-dynamodb";

const client = new DynamoDBClient({
     //Quirk for local host development only!
     region: "us-east-1",
});


export const delete_table = async () => {
  const command = new DeleteTableCommand({
    TableName: "Node_Information",
  });

  const response = await client.send(command);
  console.log("deleting old table...");
  return response;
};
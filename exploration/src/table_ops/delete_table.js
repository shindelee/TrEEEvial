import { DeleteTableCommand, DynamoDBClient } from "@aws-sdk/client-dynamodb";

const client = new DynamoDBClient({
  region: "x",
  endpoint: "http://localhost:8000",
  credentials: {
    accessKeyId: "abcd",
    secretAccessKey: "1234",
  },
});


export const delete_table = async () => {
  const command = new DeleteTableCommand({
    TableName: "Node_Information",
  });

  const response = await client.send(command);
  console.log("deleting old table...");
  return response;
};
import { ListTablesCommand, DynamoDBClient } from "@aws-sdk/client-dynamodb";

const client = new DynamoDBClient({
  region: "us-east-1",
});

export const table_list = async () => {
  const command = new ListTablesCommand({});

  const response = await client.send(command);
//   console.log(response);
  return response;
};


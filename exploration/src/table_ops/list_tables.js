import { ListTablesCommand, DynamoDBClient } from "@aws-sdk/client-dynamodb";

const client = new DynamoDBClient({
  region: "eu-north-1",
});

export const table_list = async () => {
  const command = new ListTablesCommand({});

  const response = await client.send(command);
//   console.log(response);
  return response;
};


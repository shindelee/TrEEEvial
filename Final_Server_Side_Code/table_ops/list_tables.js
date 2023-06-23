import { ListTablesCommand, DynamoDBClient } from "@aws-sdk/client-dynamodb";

const client = new DynamoDBClient({
  region: "eu-north-1",
  // region: "x",
  // endpoint: "http://localhost:8000",
  // credentials: {
  // accessKeyId: "abcd",
  // secretAccessKey: "1234",
  // },
});

export const table_list = async () => {
  const command = new ListTablesCommand({});

  const response = await client.send(command);
//   console.log(response);
  return response;
};


import { CreateTableCommand, DynamoDBClient } from "@aws-sdk/client-dynamodb";

const client = new DynamoDBClient({
  //Quirk for local host development only!
    region: "x",
    endpoint: "http://localhost:8000",
    credentials: {
      accessKeyId: "abcd",
      secretAccessKey: "1234",
    },
});

export const main = async () => {
  const command = new CreateTableCommand({
    TableName: "EspressoDrinks",
    
    AttributeDefinitions: [
      {
        AttributeName: "DrinkName",
        AttributeType: "S",
      },
    ],
    KeySchema: [
      {
        AttributeName: "DrinkName",
        KeyType: "HASH",
      },
    ],
    ProvisionedThroughput: {
      ReadCapacityUnits: 1,
      WriteCapacityUnits: 1,
    },
  });

  const response = await client.send(command);
  console.log(response);
  return response;
};

main();


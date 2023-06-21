import { CreateTableCommand, DynamoDBClient } from "@aws-sdk/client-dynamodb";

const client = new DynamoDBClient({
    // region: "eu-north-1",
    region: "x",
    endpoint: "http://localhost:8000",
    credentials: {
    accessKeyId: "abcd",
    secretAccessKey: "1234",
    },
});

export const create_table = async () => {
  const command = new CreateTableCommand({
    TableName: "Node_Information",
    // For more information about data types,
    // see https://docs.aws.amazon.com/amazondynamodb/latest/developerguide/HowItWorks.NamingRulesDataTypes.html#HowItWorks.DataTypes and
    // https://docs.aws.amazon.com/amazondynamodb/latest/developerguide/Programming.LowLevelAPI.html#Programming.LowLevelAPI.DataTypeDescriptors
    AttributeDefinitions: [
      {
        AttributeName: "x",
        AttributeType: "N",
      },

      {
        AttributeName: "y",
        AttributeType: "N",
      },


    ],
    KeySchema: [
      {
        AttributeName: "x",
        KeyType: "HASH",
      },
      {
        AttributeName: "y",
        KeyType: "RANGE",
      },
    ],
    ProvisionedThroughput: {
      ReadCapacityUnits: 1,
      WriteCapacityUnits: 1,
    },
  });

  const response = await client.send(command);
  console.log("creating new table...");
  return response;
};

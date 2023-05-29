//Create DynamoDB table

import { DeleteTableCommand, CreateTableCommand, DynamoDBClient } from "@aws-sdk/client-dynamodb";

const client = new DynamoDBClient({});


export const exists = async () => {
    const command = new DescribeTableCommand({
      TableName: "Node Information",
    });

    const response = await client.send(command);
    return response.Table.TableName ==  "Node Information"
};

export const delete_table = async () => {
    const command = new DeleteTableCommand({
      TableName: "DecafCoffees",
    });
  
    const response = await client.send(command);
    console.log(response);
    return response;
  };



export const create_table = async () => {
  const command = new CreateTableCommand({
    TableName: "Node Information",

    AttributeDefinitions: [
      {AttributeName: "Coordinate", AttributeType: "N",},
      {AttributeName: "Origin", AttributeType: "N"},
      {AttributeName: "Visited", AttributeType: "N"},
    ],

    KeySchema: [
      {
        AttributeName: "Coordinate",
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








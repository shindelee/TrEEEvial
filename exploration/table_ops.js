//Create DynamoDB table

import { DeleteTableCommand, CreateTableCommand, DynamoDBClient } from "@aws-sdk/client-dynamodb";
import { PutCommand, UpdateCommand, DynamoDBDocumentClient } from "@aws-sdk/lib-dynamodb";

const client = new DynamoDBClient({});
const docClient = DynamoDBDocumentClient.from(client);
const TABLE_NAME = "Node Information";


export const exists = async () => {
    const command = new DescribeTableCommand({
      TableName: TABLE_NAME,
    });

    const response = await client.send(command);
    return response.Table.TableName ==  TABLE_NAME;
};

export const delete_table = async () => {
    const command = new DeleteTableCommand({
      TableName: TABLE_NAME,
    });
  
    const response = await client.send(command);
    console.log(response);
    return response;
  };


//Note: Coordinate is string, need to do string processing
//to extract x and y coordinate

export const create_table = async () => {
  const command = new CreateTableCommand({
    TableName: TABLE_NAME,

    AttributeDefinitions: [
      {AttributeName: "Coordinate", AttributeType: "S",},
      {AttributeName: "Type", AttributeType: "N",}, 
      {AttributeName: "Origin", AttributeType: "N"},
      {AttributeName: "Visited", AttributeType: "N"},
      {AttributeName: "Decision_Node", AttributeType: "N"}, //if true, this node 
      //has multiple possible decisions
      {AttributeName: "Start_Node", AttributeType: "N"},
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


export const add_node = async (coordinate, origin, visited, decision, start) => {
  const command = new PutCommand({
    TableName: TABLE_NAME,
    Item: {
      Coordinate : coordinate,
      Type : 0, //path
      Origin : origin,
      Visited : visited,
      Decision_Node : decision,
      Start_Node : start,
    },
  });

  const response = await docClient.send(command);
  console.log(response);
  return response;
};

export const add_wall = async (coordinate) => {
  const command = new PutCommand({
    TableName: TABLE_NAME,
    Item: {
      Coordinate : coordinate,
      Type : 1, //wall
      Origin : null,
      Visited : null,
      Decision_Node : null,
      Start_Node : null,
    },
  });

  const response = await docClient.send(command);
  console.log(response);
  return response;
};

export const edit_entry = async (coordinate) => {
  const command = new UpdateCommand({
    TableName: TABLE_NAME,
    Key: {
      Coordinate: coordinate,
    },
    UpdateExpression: "set Color = :color",
    ExpressionAttributeValues: {
      ":color": "black",
    },
    ReturnValues: "ALL_NEW",
  });

  const response = await docClient.send(command);
  console.log(response);
  return response;
};

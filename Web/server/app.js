import express from "express";
import challenges, { gameOver } from "./public/challenges.js";
import { generateUniqueRandomNum } from "./libs/util.js";

const app = express();
const port = 3003;
// Subscribed client to server sent events
let clientRes = null;
let client = {
  currentChallenge: null,
  score: 0,
  health: 3,
  isAlive: true,
};

app.use(express.urlencoded({ extended: true }));
app.use(express.static("public"));

if (process.env.NODE_ENV === "production") {
  app.use(express.static("client/build"));

  const path = require("path");
  app.get("*", (req, res) => {
    res.sendFile(path.resolve(__dirname, "client", "build", "index.html"));
  });
}

// Bypass CORS to allow fetching data
app.use((req, res, next) => {
  res.header("Access-Control-Allow-Origin", "*");
  res.header(
    "Access-Control-Allow-Headers",
    "Origin, X-Requested-With, Content-Type, Accept"
  );
  next();
});

// GET a random challenge
const getRandomChallenge = (req, res) => {
  if (!client.isAlive) {
    client.isAlive = true;
    client.health = 3;
    client.score = 0;
  }

  client.currentChallenge = challenges[generateUniqueRandomNum()];
  return res.status(200).send(client);
};

// Subscribe to server sent events
const addSubscriber = (req, res) => {
  const headers = {
    "Content-Type": "text/event-stream",
    Connection: "keep-alive",
  };
  res.writeHead(200, headers);

  clientRes = res;
};

// Send a new random challenge
const sendNextChallenge = (isAlive) => {
  if (isAlive) {
    client.currentChallenge = challenges[generateUniqueRandomNum()];
  } else {
    client.currentChallenge = gameOver;
  }

  clientRes.write("data:" + JSON.stringify(client));
  clientRes.write("\n\n");
};

// Handle MCU POST request
const handleMCUPost = (req, res) => {
  console.log(req.body.mcu);
  console.log(req.body.cmd);

  const cmd = req.body.cmd;

  if (cmd === client.currentChallenge.cmd) {
    client.score++;
  } else {
    client.health--;
    if (client.health === 0) {
      client.isAlive = false;
    }
  }
  sendNextChallenge(client.isAlive);

  return res.status(200).send({
    error: false,
  });
};

// Define endpoints
app.get("/api/challenges", getRandomChallenge);
app.get("/api/subscribe", addSubscriber);
app.post("/api/challenges", handleMCUPost);

// Start the app
app.listen(port, () => {
  console.log(`Example app listening on port ${port}`);
});

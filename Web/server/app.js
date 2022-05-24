import express from "express";
import challenges from "./public/challenges.js";
import { generateUniqueRandomNum } from "./libs/util.js";

const app = express();
const port = 3003;

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
app.use(function(req, res, next) {
  res.header('Access-Control-Allow-Origin', '*');
  res.header('Access-Control-Allow-Headers', 'Origin, X-Requested-With, Content-Type, Accept');
  next();
});

// GET a random challenge - prevents getting the same challenge twice
app.get(`/api/challenges`, async (req, res) => {
  return res.status(200).send(challenges[generateUniqueRandomNum()]);
});

// POST route for testing purposes only atm - logs the mcu and cmd sending the request
app.post(`/api/challenges`, async (req, res) => {
  console.log(req.body.mcu);
  console.log(req.body.cmd);

  return res.status(200).send({
    error: false,
  })
})


app.listen(port, () => {
  console.log(`Example app listening on port ${port}`);
});

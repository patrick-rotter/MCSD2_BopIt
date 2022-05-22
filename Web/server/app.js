import express from "express";
import challenges from "./public/challenges.js";
import { generateUniqueRandomNum } from "./libs/util.js";

const app = express();
const port = 6000;

app.use(express.urlencoded({ extended: true }));
app.use(express.static("public"));

if (process.env.NODE_ENV === "production") {
  app.use(express.static("client/build"));

  const path = require("path");
  app.get("*", (req, res) => {
    res.sendFile(path.resolve(__dirname, "client", "build", "index.html"));
  });
}

// GET a random challenge - prevents getting the same challenge twice
app.get(`/api/challenges`, async (req, res) => {
  return res.status(200).send(challenges[generateUniqueRandomNum()]);
});

app.listen(port, () => {
  console.log(`Example app listening on port ${port}`);
});

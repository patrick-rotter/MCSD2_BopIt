import express from "express";
import challenges from "./public/challenges.js";

const app = express();
const port = 5000;
let randomNumber = 0;
/* Initialize previous number with array.length + 1 
to make sure every challenge can occur at the start */
let previousNumber = challenges.length + 1;

app.use(express.urlencoded({ extended: true }));
app.use(express.static("public"));

// Generate a random number (but different from previous random nubmer)
const generateUniqueRandomNum = () => {
  let isPreviousNumber = true;

  while (isPreviousNumber) {
    randomNumber = Math.floor(Math.random() * challenges.length);
    if (randomNumber !== previousNumber) {
      isPreviousNumber = false;
    }
  }

  previousNumber = randomNumber;
  return randomNumber;
};

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

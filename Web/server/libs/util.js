import challenges from "../public/challenges.js";

let randomNumber = 0;
/* Initialize previous number with array.length + 1 
to make sure every challenge can occur at the start */
let previousNumber = challenges.length + 1;

// Generate a random number (but different from previous random nubmer)
export const generateUniqueRandomNum = () => {
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

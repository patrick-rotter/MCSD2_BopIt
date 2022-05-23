import React from "react";
import Title from "./Title";
import Image from "./Image";
import Description from "./Description";

// Demo data
let demoCard = {
  title: "Pet the cat",
  img: "https://cdn.britannica.com/91/181391-050-1DA18304/cat-toes-paw-number-paws-tiger-tabby.jpg?q=60",
  description: "Lorem ipsum dolor sit amet, consectetur adipiscing elit.",
};

const ChallengeCard = () => {
  return (
    <div className="card">
      <Title text={demoCard.title} />
      <Image url={demoCard.img} />
      <Description text={demoCard.description} />
    </div>
  );
};

export default ChallengeCard;

import React, { useState } from "react";
import Title from "./Title";
import Image from "./Image";
import Description from "./Description";

const ChallengeCard = () => {
  const [challenge, setChallenge] = useState({
    mcu: "",
    img: "",
    description: "",
  });

  // Get a random challenge from server (TODO: call periodically instead of button press)
  const handleClick = () => {
    fetch("http://localhost:3003/api/challenges", {
      method: "GET",
    })
      .then((res) => res.json())
      .then((data) =>
        setChallenge({
          mcu: data.module,
          img: data.img,
          description: data.description,
        })
      );
  };

  return (
    <div className="card">
      <Title text={challenge.mcu} />
      <Image url={challenge.img} />
      <Description text={challenge.description} />
      <button onClick={handleClick}>Click</button>
    </div>
  );
};

export default ChallengeCard;

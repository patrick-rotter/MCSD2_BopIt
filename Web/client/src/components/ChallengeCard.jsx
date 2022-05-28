import React, { useEffect, useState } from "react";
import Title from "./Title";
import Image from "./Image";
import Description from "./Description";

const ChallengeCard = () => {
  const [challenge, setChallenge] = useState({
    mcu: "",
    img: "",
    description: "",
  });

  const fetchChallenge = () => {
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

  useEffect(() => {
    fetchChallenge();
    const eventSource = new EventSource("http://localhost:3003/api/subscribe");
    eventSource.onmessage = (e) => {
      const eventData = JSON.parse(e.data)
      console.log(eventData);
      setChallenge({
        mcu: eventData.module,
        img: eventData.img,
        description: eventData.description,
      })
    }

    return () => {
      eventSource.close();
    };
  }, []);

  return (
    <div className="card">
      <Title text={challenge.mcu} />
      <Image url={challenge.img} />
      <Description text={challenge.description} />
    </div>
  );
};

export default ChallengeCard;

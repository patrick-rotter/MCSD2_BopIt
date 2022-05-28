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

  // Rerenders the challenge card
  const updateChallenge = (data) => {
    setChallenge({
      mcu: data.module,
      img: data.img,
      description: data.description,
    })
  }

  // Fetches a random challenge
  const fetchChallenge = () => {
    fetch("http://localhost:3003/api/challenges", {
      method: "GET",
    })
      .then((res) => res.json())
      .then((data) => updateChallenge(data)
      );
  };

  // Subscribes to sse to update the challenge on a sent event
  useEffect(() => {
    fetchChallenge();
    const eventSource = new EventSource("http://localhost:3003/api/subscribe");
    eventSource.onmessage = (e) => {
      const eventData = JSON.parse(e.data)
      console.log(eventData);
      updateChallenge(eventData)
    }
    return () => {
      eventSource.close();
    };
    // eslint-disable-next-line react-hooks/exhaustive-deps
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

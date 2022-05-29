import React, { useEffect, useState } from "react";
import Title from "./Title";
import Image from "./Image";
import Description from "./Description";
import StatusBar from "./StatusBar";

const ChallengeCard = () => {
  const [challenge, setChallenge] = useState({
    mcu: "",
    img: "",
    description: "",
    cmd: "",
  });
  const [health, setHealth] = useState(3);
  const [score, setScore] = useState(0);
  const [isAlive, setIsAlive] = useState(true);

  // Rerenders the challenge card
  const updateChallenge = (data) => {
    setChallenge({
      mcu: data.currentChallenge.module,
      img: data.currentChallenge.img,
      description: data.currentChallenge.description,
      cmd: data.currentChallenge.cmd,
    });
  };

  // Fetches a random challenge
  const fetchChallenge = () => {
    fetch("http://localhost:3003/api/challenges", {
      method: "GET",
    })
      .then((res) => res.json())
      .then((data) => updateChallenge(data));
  };

  const resetGame = () => {
    fetchChallenge();
    setHealth(3);
    setScore(0);
    setIsAlive(true);
  };

  // Subscribes to sse to update the challenge on a sent event
  useEffect(() => {
    fetchChallenge();
    const eventSource = new EventSource("http://localhost:3003/api/subscribe");
    eventSource.onmessage = (e) => {
      const eventData = JSON.parse(e.data);
      console.log(eventData);
      updateChallenge(eventData);
      setScore(eventData.score);
      setHealth(eventData.health);
      setIsAlive(eventData.isAlive);
    };
    return () => {
      eventSource.close();
    };
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, []);

  return (
    <div className="card">
      <StatusBar points={score} lives={health} />
      <Title text={challenge.mcu} />
      <Image url={challenge.img} />
      <Description text={challenge.description} />
      <p>{challenge.cmd}</p>
      {!isAlive && <button onClick={resetGame}>Play again?</button>}
    </div>
  );
};

export default ChallengeCard;

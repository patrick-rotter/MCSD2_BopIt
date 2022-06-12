import React, { useEffect, useState } from "react";
import Title from "./Title";
import Image from "./Image";
import Description from "./Description";
import StatusBar from "./StatusBar";
import { motion, AnimatePresence } from "framer-motion";

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
  const [isHidden, setIsHidden] = useState(false);

  // Rerenders the challenge card
  const updateChallenge = (data) => {
    setChallenge({
      mcu: data.currentChallenge.module,
      img: data.currentChallenge.img,
      description: data.currentChallenge.description,
      cmd: data.currentChallenge.cmd,
    });
  };

  // TODO: Put in custom hook
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

  // Change isHidden state to fade challenge out and in again
  const fadeOutAndIn = () => {
    setIsHidden(true);
    setTimeout(() => {
      setIsHidden(false);
    }, 1500);
  };

  // Subscribes to sse to update the challenge on a sent event
  useEffect(() => {
    fetchChallenge();
    const eventSource = new EventSource("http://localhost:3003/api/subscribe");
    eventSource.onmessage = (e) => {
      fadeOutAndIn();

      const eventData = JSON.parse(e.data);
      console.log(eventData);
      updateChallenge(eventData);
      setScore(eventData.score);
      eventData.health > 0 ? setHealth(eventData.health) : setHealth(0);
      setIsAlive(eventData.isAlive);
    };
    return () => {
      eventSource.close();
    };
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, []);

  return (
    <div className="container">
      <div className="card">
        <StatusBar points={score} health={health} />

        <AnimatePresence>
          {!isHidden && (
            <motion.div
            className="challenge"
              key={challenge.cmd}
              initial={{ opacity: 0 }}
              animate={{ opacity: 1 }}
              exit={{ opacity: 0 }}
            >
              <Title text={challenge.mcu} cmd={challenge.cmd} isAlive={isAlive} />
              <Image url={challenge.img} />
              <Description text={challenge.description} />
              <p>{challenge.cmd}</p>
            </motion.div>
          )}
        </AnimatePresence>

      </div>
      {!isAlive && (
        <button className="reset-btn" onClick={resetGame}>
          Play again?
        </button>
      )}
    </div>
  );
};

export default ChallengeCard;

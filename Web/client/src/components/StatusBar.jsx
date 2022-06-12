import React from "react";
import { motion, AnimatePresence } from "framer-motion";

const StatusBar = (props) => {
  return (
    <div className="status-bar">
      <div className="score-el">Score: {props.points}</div>
      <div className="pixel-heart-container">
        {Array(props.health)
          .fill(0)
          .map((_, i) => (
            <AnimatePresence>
              <motion.div
                key={i}
                initial={{ opacity: 0 }}
                animate={{ opacity: 1 }}
                exit={{ opacity: 0 }}
              >
                <img
                  className="pixel-heart-img"
                  src={process.env.PUBLIC_URL + "/pixel-heart.png"}
                  alt="pixel-heart"
                ></img>
              </motion.div>
            </AnimatePresence>
          ))}
      </div>
    </div>
  );
};

export default StatusBar;

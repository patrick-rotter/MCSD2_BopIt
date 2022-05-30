import React from "react";

const StatusBar = (props) => {
  return (
    <div className="status-bar">
      <span>Score: {props.points}</span>
      <div>
        {Array(props.health)
          .fill(0)
          .map((_, i) => (
            <img
              src={process.env.PUBLIC_URL + "/pixel-heart.png"}
              alt="pixel-heart"
              height="40"
              widht="40"
            ></img>
          ))}
      </div>
    </div>
  );
};

export default StatusBar;
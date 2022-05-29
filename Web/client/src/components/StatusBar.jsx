import React from "react";

const StatusBar = (props) => {
  return (
    <div className="status-bar">
      <span>Score: {props.points}</span>
      <span>Health: {props.lives}</span>
    </div>
  );
};

export default StatusBar;

import React from 'react';
import '../../App.css';
// import video1 from "../../video.mp4";
// import './Balancing.css'

function Balancing() {

  return (
    <div className='video-container'>
      <h1 className='Balancing'>WATCH THE ROVER BALANCING</h1>
      <div className='video-wrapper'>
        <video className='centered-video' controls >
          <source src='../../video.mp4' type='video/mp4' />
          Your browser does not support the video tag.
        </video>
      </div>
    </div>
    );
}
export default Balancing;
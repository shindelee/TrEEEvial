import React from 'react';
import Navbar from './components/Navbar';
import './App.css';
import Home from './components/pages/Home';
import { BrowserRouter as Router, Routes, Route } from 'react-router-dom';
import Maze from './components/pages/Maze';
import Services from './components/pages/Services';
import SignUp from './components/pages/SignUp';

function App() {
  return (
    <>
      <Router>
        <Navbar />
        <Routes>
          <Route path='/' exact component={<Home/>} />
          <Route path='/services' component={<Services/>} />
          <Route path='/maze' component={<Maze/>} />
          <Route path='/sign-up' component={<SignUp/>} />
        </Routes>
      </Router>
    </>
  );
}

export default App;
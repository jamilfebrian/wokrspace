import { useState } from 'react'
import reactLogo from './assets/react.svg'
import viteLogo from '/vite.svg'
import './App.css'
import Color from './color'

function App() {
  const [count, setCount] = useState(0)

  return (
    <>
      <Color />
    </>
  )
}

export default App

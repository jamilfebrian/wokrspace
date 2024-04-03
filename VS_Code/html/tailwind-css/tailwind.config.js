/** @type {import('tailwindcss').Config} */
module.exports = {
  content: ["./public/**/*.{html,js}"],
  darkMode: 'class',
  theme: {
    extend: {
      colors: {
        merah : '#ef4444',
        tomato : '#dc2626',
        janda : '#4a044e'
      },
      spacing : {
        13 : '3.25rem',
      },
      fontFamily: {
        ojuju : ['Ojuju'],
        roboto : ['Roboto']
      },
      animation: {
        'spin-slow' : 'spin 3s linear infinite',
        'goyang' : 'wiggle 1s ease-in-out infinite'
      },
      keyframes: {
        wiggle:{
          '0%, 100%' : {transform : 'rotate(-3deg)'},
          '50%' : {transform : 'rotate(3deg)'}
        }
      }
    },
  },
  plugins: [],
}


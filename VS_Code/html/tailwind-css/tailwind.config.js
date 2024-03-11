/** @type {import('tailwindcss').Config} */
module.exports = {
  content: ["./public/**/*.{html,js}"],
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
      fontFamily:{
        ojuju : ['Ojuju'],
        roboto : ['Roboto']
      },
    },
  },
  plugins: [],
}


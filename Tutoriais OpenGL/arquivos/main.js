document.addEventListener('DOMContentLoaded', () => {
  document.querySelectorAll('pre').forEach(preElement => {
    preElement.addEventListener('click', async (event) => {
      // Get the coordinates of the click
      const rect = preElement.getBoundingClientRect();
      const x = event.clientX - rect.left;
      const y = event.clientY - rect.top;

      // Check if the click is within the pseudo-element's area (you need to define this)
      // This is less reliable due to CSS positioning variables. A better method follows.

      // Better approach: assume any click within the top-right corner is meant for the "Copy" area
      if (x > rect.width - 60 && y < 40) { // Adjust these numbers based on your CSS
        const textToCopy = preElement.textContent;

        try {
          await navigator.clipboard.writeText(textToCopy);
          preElement.classList.add('copied');
          setTimeout(() => {
            preElement.classList.remove('copied');
          }, 2000);
        } catch (err) {
          console.error('Failed to copy text: ', err);
        }
      }
    });
  });
});
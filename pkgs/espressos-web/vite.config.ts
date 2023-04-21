import { defineConfig } from "vite";
import solidPlugin from "vite-plugin-solid";

export default defineConfig({
  plugins: [solidPlugin()],
  server: {
    port: 3000,
    proxy: {
      "/api": {
        target: "ws://10.1.1.137",
        ws: true,
      },
    },
  },
  build: {
    target: "esnext",
  },
});

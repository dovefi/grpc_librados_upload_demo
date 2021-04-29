// Go File
// FILE: client.go
// AUTHOR: dovefi
// ROLE:
// CREATED: 29/04/2021 11:15 AM

package main

import (
	"bufio"
	"context"
	"fmt"
	"github.com/dovefi/grpc_librados_upload_demo/client/pb"
	"google.golang.org/grpc"
	"io"
	"os"
)

var (
	grpcServerHost = "127.0.0.1:6888"
)

func UploadFromLocalFile(fileName string) error {
	conn, err := grpc.Dial(grpcServerHost, grpc.WithInsecure())
	if err != nil {
		panic(err)
	}
	client := pb.NewLaptopServiceClient(conn)

	stream, err := client.UploadFile(context.Background())
	if err != nil {
		return err
	}
	file, err := os.Open(fileName)
	if err != nil {
		return err
	}
	defer file.Close()

	// send file info first
	req := &pb.UploadFileRequest{
		Data: &pb.UploadFileRequest_Info{
			Info: &pb.FileInfo{
				Name: fileName,
			},
		},
	}

	err = stream.Send(req)
	if err != nil {
		fmt.Println(fmt.Errorf("stream send file info fail %w", err))
		return err
	}

	reader := bufio.NewReader(file)
	buffer := make([]byte, 1024)
	for {
		n, err := reader.Read(buffer)
		if err == io.EOF {
			break
		}
		req := &pb.UploadFileRequest{
			Data: &pb.UploadFileRequest_ChunkData{
				ChunkData: buffer[:n],
			},
		}
		err = stream.Send(req)
		if err != nil {
			fmt.Println("stream send fail")
		}
	}
	res, err := stream.CloseAndRecv()
	if err != nil {
		fmt.Println(err)
		return err
	}
	fmt.Printf("upload file: %s success \n ", res.GetId())
	return nil
}

func main() {
	err := UploadFromLocalFile(os.Args[1])
	if err != nil {
		panic(err)
	}
}
